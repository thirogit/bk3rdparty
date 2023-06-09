
//******************************************************************************
// RCF - Remote Call Framework
//
// Copyright (c) 2005 - 2013, Delta V Software. All rights reserved.
// http://www.deltavsoft.com
//
// RCF is distributed under dual licenses - closed source or GPL.
// Consult your particular license for conditions of use.
//
// If you have not purchased a commercial license, you are using RCF 
// under GPL terms.
//
// Version: 2.0
// Contact: support <at> deltavsoft.com 
//
//******************************************************************************

#include <RCF/PublishingService.hpp>

#include <RCF/CurrentSession.hpp>
#include <RCF/MulticastClientTransport.hpp>
#include <RCF/RcfServer.hpp>
#include <RCF/RcfSession.hpp>
#include <RCF/ServerInterfaces.hpp>
#include <RCF/ServerTransport.hpp>
#include <RCF/ThreadLibrary.hpp>

namespace RCF {

    void PublisherParms::setTopicName(const std::string & topicName)
    {
        mTopicName = topicName;
    }

    std::string PublisherParms::getTopicName() const
    {
        return mTopicName;
    }

    void PublisherParms::setOnSubscriberConnect(OnSubscriberConnect onSubscriberConnect)
    {
        mOnSubscriberConnect = onSubscriberConnect;
    }

    void PublisherParms::setOnSubscriberDisconnect(OnSubscriberDisconnect onSubscriberDisconnect)
    {
        mOnSubscriberDisconnect = onSubscriberDisconnect;
    }
    
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4355 ) // warning C4355: 'this' : used in base member initializer list
#endif

    PublishingService::PublishingService() :
        mPingIntervalMs(0),
        mPeriodicTimer(*this, 0)
    {}

#ifdef _MSC_VER
#pragma warning( pop )
#endif

    PublishingService::~PublishingService()
    {
    }

    void PublishingService::setPingIntervalMs(boost::uint32_t pingIntervalMs)
    {
        mPingIntervalMs = pingIntervalMs;
    }

    boost::uint32_t PublishingService::getPingIntervalMs() const
    {
        return mPingIntervalMs;
    }

    // remotely accessible

    boost::int32_t PublishingService::RequestSubscription(
        const std::string &subscriptionName)
    {
        boost::uint32_t subToPubPingIntervalMs = 0;
        boost::uint32_t pubToSubPingIntervalMs = 0;

        return RequestSubscription(
            subscriptionName, 
            subToPubPingIntervalMs, 
            pubToSubPingIntervalMs);
    }

    boost::int32_t PublishingService::RequestSubscription(
        const std::string &subscriptionName,
        boost::uint32_t subToPubPingIntervalMs,
        boost::uint32_t & pubToSubPingIntervalMs)
    {
        PublisherPtr publisherPtr;
        std::string publisherName = subscriptionName;
        Lock lock(mPublishersMutex);
        Publishers::iterator iter = mPublishers.find(publisherName);
        if (iter != mPublishers.end())
        {
            PublisherWeakPtr publisherWeakPtr = iter->second;
            publisherPtr = publisherWeakPtr.lock();
        }
        lock.unlock();
        if (publisherPtr)
        {
            RcfSession & rcfSession = getTlsRcfSession();

            if (publisherPtr->mParms.mOnSubscriberConnect)
            {
                bool allowSubscriber = publisherPtr->mParms.mOnSubscriberConnect(rcfSession, subscriptionName);
                if (!allowSubscriber)
                {
                    return RcfError_AccessDenied;
                }
            }            

            ServerTransportEx &serverTransport =
                dynamic_cast<ServerTransportEx &>(
                    rcfSession.getSessionState().getServerTransport());

            ClientTransportAutoPtrPtr clientTransportAutoPtrPtr(
                new ClientTransportAutoPtr(
                    serverTransport.createClientTransport(
                        rcfSession.shared_from_this())));

            (*clientTransportAutoPtrPtr)->setRcfSession(
                rcfSession.shared_from_this());

            rcfSession.setPingIntervalMs(subToPubPingIntervalMs);

            if (rcfSession.isInProcess())
            {
                addSubscriberTransport(
                    rcfSession, 
                    publisherName, 
                    clientTransportAutoPtrPtr);
            }
            else
            {
                rcfSession.addOnWriteCompletedCallback(
                    boost::bind(
                        &PublishingService::addSubscriberTransport,
                        this,
                        _1,
                        publisherName,
                        clientTransportAutoPtrPtr) );
            }

            if (publisherPtr->mParms.mOnSubscriberDisconnect)
            {
                rcfSession.setOnDestroyCallback( boost::bind(
                    publisherPtr->mParms.mOnSubscriberDisconnect, 
                    _1, 
                    subscriptionName));
            }            
        }  
        pubToSubPingIntervalMs = mPingIntervalMs;
        return publisherPtr ? RcfError_Ok : RcfError_UnknownPublisher;
    }

    void PublishingService::onServiceAdded(RcfServer & server)
    {
        server.bind<I_RequestSubscription>(*this);
    }

    void PublishingService::onServiceRemoved(RcfServer &server)
    {
        server.unbind<I_RequestSubscription>();
    }

    void PublishingService::onServerStart(RcfServer &server)
    {
        RCF_UNUSED_VARIABLE(server);
        mPeriodicTimer.setIntervalMs(mPingIntervalMs);
        mPeriodicTimer.start();
    }

    void PublishingService::onServerStop(RcfServer &server)
    {
        RCF_UNUSED_VARIABLE(server);
        mPeriodicTimer.stop();

        // Close all publishers.

        Publishers publishers;
        {
            Lock writeLock(mPublishersMutex);
            publishers = mPublishers;
        }

        Publishers::iterator iter;
        for (iter = publishers.begin(); iter != publishers.end(); ++iter)
        {
            PublisherPtr publisherPtr = iter->second.lock();
            if (publisherPtr)
            {
                publisherPtr->close();
            }
        }

        {
            Lock writeLock(mPublishersMutex);
            RCF_ASSERT(mPublishers.empty());
        }
    }

    void PublishingService::addSubscriberTransport(
        RcfSession &session,
        const std::string &publisherName,
        ClientTransportAutoPtrPtr clientTransportAutoPtrPtr)
    {
        session.setPingTimestamp();

        Lock lock(mPublishersMutex);
        if (mPublishers.find(publisherName) != mPublishers.end())
        {
            PublisherPtr publisherPtr = mPublishers[ publisherName ].lock();
            if (publisherPtr)
            {
                ClientTransport &clientTransport =
                    publisherPtr->mRcfClientPtr->getClientStub().getTransport();

                MulticastClientTransport &multiCastClientTransport =
                    dynamic_cast<MulticastClientTransport &>(clientTransport);

                multiCastClientTransport.addTransport(*clientTransportAutoPtrPtr);
            }
        }
    }

    void PublishingService::closePublisher(const std::string & name)
    {
        Lock writeLock(mPublishersMutex);
        Publishers::iterator iter = mPublishers.find(name);
        if (iter != mPublishers.end())
        {
            mPublishers.erase(iter);
        }
    }

    void PublishingService::onTimer()
    {
        pingAllSubscriptions();
        harvestExpiredSubscriptions();
    }

    void PublishingService::pingAllSubscriptions()
    {
        // Send oneway pings on all our subscriptions, so the subscriber
        // knows we're still alive.

        Lock lock(mPublishersMutex);

        Publishers::iterator iter;
        for (iter = mPublishers.begin(); iter != mPublishers.end(); ++iter)
        {
            PublisherPtr publisherPtr = iter->second.lock();
            if (publisherPtr)
            {
                PublisherBase & pub = * publisherPtr;

                ClientTransport & transport = 
                    pub.mRcfClientPtr->getClientStub().getTransport();

                MulticastClientTransport & multiTransport = 
                    static_cast<MulticastClientTransport &>(transport);

                multiTransport.pingAllTransports();
            }
        }
    }

    void PublishingService::harvestExpiredSubscriptions()
    {
        // Kill off subscriptions that haven't received any recent pings.

        Lock lock(mPublishersMutex);

        Publishers::iterator iter;
        for (iter = mPublishers.begin(); iter != mPublishers.end(); ++iter)
        {
            PublisherPtr publisherPtr = iter->second.lock();
            if (publisherPtr)
            {
                PublisherBase & pub = * publisherPtr;

                ClientTransport & transport = 
                    pub.mRcfClientPtr->getClientStub().getTransport();

                MulticastClientTransport & multiTransport = 
                    static_cast<MulticastClientTransport &>(transport);

                multiTransport.dropIdleTransports();
            }
        }
    }

    PublisherBase::PublisherBase(PublishingService & pubService, const PublisherParms & parms) : 
        mPublishingService(pubService),
        mParms(parms),
        mClosed(false)
    {
        mTopicName = parms.getTopicName();
    }

    PublisherBase::~PublisherBase()
    {
        if (!mClosed)
        {
            close();
        }
    }

    std::string PublisherBase::getTopicName()
    {
        return mTopicName;
    }

    void PublisherBase::close()
    {
        mPublishingService.closePublisher(mTopicName);
        mRcfClientPtr->getClientStub().releaseTransport();
        mClosed = true;
    }

    void PublisherBase::init()
    {
        mRcfClientPtr->getClientStub().setTransport(
            ClientTransportAutoPtr(new MulticastClientTransport));

        mRcfClientPtr->getClientStub().setRemoteCallSemantics(Oneway);
        mRcfClientPtr->getClientStub().setTargetName("");
        mRcfClientPtr->getClientStub().setTargetToken( Token());
    }

} // namespace RCF
