
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

#ifndef INCLUDE_RCF_BSDCLIENTTRANSPORT_HPP
#define INCLUDE_RCF_BSDCLIENTTRANSPORT_HPP

#include <RCF/Export.hpp>
#include <RCF/ConnectionOrientedClientTransport.hpp>

#include <boost/scoped_ptr.hpp>

namespace RCF {

    typedef AsioSocket                              TcpSocket;
    typedef boost::shared_ptr<TcpSocket>            TcpSocketPtr;

#ifdef RCF_HAS_LOCAL_SOCKETS

    using ASIO_NS::local::stream_protocol;
    typedef stream_protocol::socket                 UnixLocalSocket;
    typedef boost::shared_ptr<UnixLocalSocket>      UnixLocalSocketPtr;

#else

    typedef TcpSocket                               UnixLocalSocket;
    typedef TcpSocketPtr                            UnixLocalSocketPtr;

#endif

    class RCF_EXPORT BsdClientTransport :
        public ConnectionOrientedClientTransport
    {
    public:

        BsdClientTransport();
        BsdClientTransport(const BsdClientTransport & rhs);
        BsdClientTransport(TcpSocketPtr socketPtr);

#ifdef RCF_HAS_LOCAL_SOCKETS
        BsdClientTransport(UnixLocalSocketPtr socketPtr);
#endif

        ~BsdClientTransport();
        
        TcpSocketPtr            releaseTcpSocket();
        UnixLocalSocketPtr      releaseLocalSocket();

        int                     getNativeHandle() const;

    private:

        std::size_t             implRead(
                                    const ByteBuffer &byteBuffer,
                                    std::size_t bytesRequested);

        std::size_t             implReadAsync(
                                    const ByteBuffer &byteBuffer,
                                    std::size_t bytesRequested);

        std::size_t             implWrite(
                                    const std::vector<ByteBuffer> &byteBuffers);

        std::size_t             implWriteAsync(
                                    const std::vector<ByteBuffer> &byteBuffers);
        
        bool                    isConnected();

    protected:

        int                     mFd;
        TcpSocketPtr            mTcpSocketPtr; 
        UnixLocalSocketPtr      mLocalSocketPtr;

        AsioBuffersPtr          mAsioBuffersPtr;
        AsioIoService *         mpIoService;

        int                     mWriteCounter;
    };

} // namespace RCF

#endif // ! INCLUDE_RCF_TCPCLIENTTRANSPORT_HPP
