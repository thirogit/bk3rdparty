
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

#ifndef INCLUDE_RCF_UDPSERVERTRANSPORT_HPP
#define INCLUDE_RCF_UDPSERVERTRANSPORT_HPP

#include <string>
#include <vector>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <RCF/Export.hpp>
#include <RCF/IpAddress.hpp>
#include <RCF/Service.hpp>
#include <RCF/ServerTransport.hpp>
#include <RCF/IpServerTransport.hpp>
#include <RCF/ThreadLibrary.hpp>

namespace RCF {
   
    class UdpServerTransport;
    class UdpSessionState;

    typedef boost::shared_ptr<UdpServerTransport>   UdpServerTransportPtr;
    typedef boost::shared_ptr<UdpSessionState>      UdpSessionStatePtr;

    class UdpSessionState : public SessionState
    {
    public:

        UdpSessionState(UdpServerTransport & transport);

        int                        getNativeHandle() const;

        typedef UdpSessionStatePtr SessionStatePtr;

    private:

        ReallocBufferPtr                            mReadVecPtr;
        ReallocBufferPtr                            mWriteVecPtr;
        IpAddress                                   mRemoteAddress;
        UdpServerTransport &                        mTransport;
        SessionPtr                                  mSessionPtr;

        friend class UdpServerTransport;

    private:

        // I_SessionState
        const RemoteAddress & getRemoteAddress() const;
        ServerTransport &     getServerTransport();
        const RemoteAddress & getRemoteAddress();

        void                    setTransportFilters(
                                    const std::vector<FilterPtr> &filters);

        void                    getTransportFilters(
                                    std::vector<FilterPtr> &filters);

        ByteBuffer              getReadByteBuffer();
        void                    postRead();
        
        void                    postWrite(
                                    std::vector<ByteBuffer> &byteBuffers);

        void                    postClose();      

        bool                    isConnected();
    };

    class RCF_EXPORT UdpServerTransport :
        public ServerTransport,
        public IpServerTransport,
        public I_Service,
        boost::noncopyable
    {
    public:

        UdpServerTransport(
            const IpAddress &       ipAddress, 
            const IpAddress &       multicastIpAddress = IpAddress());

        TransportType getTransportType();

        ServerTransportPtr 
                    clone();

        RcfServer &
                    getSessionManager();

        void        setSessionManager(RcfServer & sessionManager);
        int         getPort() const;
        void        open();
        void        close();
        void        cycle(int timeoutMs);

        void        cycleTransportAndServer(int timeoutMs);

        UdpServerTransport & enableSharedAddressBinding();

        // I_Service implementation
    private:
        void        onServiceAdded(RcfServer &server);
        void        onServiceRemoved(RcfServer &server);
        void        onServerStart(RcfServer &server);
        void        onServerStop(RcfServer &server);

    private:

        typedef UdpSessionState SessionState;
        typedef UdpSessionStatePtr SessionStatePtr;

        RcfServer *         mpRcfServer;
        IpAddress           mIpAddress;
        IpAddress           mMulticastIpAddress;
        int                 mFd;
        unsigned int        mPollingDelayMs;
        bool                mEnableSharedAddressBinding;

        friend class UdpSessionState;

    };

} // namespace RCF

#endif // ! INCLUDE_RCF_UDPSERVERTRANSPORT_HPP
