
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

#include <RCF/Win32NamedPipeServerTransport.hpp>

#include <RCF/Asio.hpp>
#include <RCF/RcfSession.hpp>
#include <RCF/ThreadLocalData.hpp>
#include <RCF/Win32NamedPipeClientTransport.hpp>
#include <RCF/Win32NamedPipeEndpoint.hpp>

namespace RCF {

    // Win32NamedPipeSessionState

    Win32NamedPipeSessionState::Win32NamedPipeSessionState(
        Win32NamedPipeServerTransport & transport,
        AsioIoService & ioService) :
            AsioSessionState(transport, ioService)
    {
        const std::size_t       MaxPipeInstances    = PIPE_UNLIMITED_INSTANCES;
        const DWORD             OutBufferSize       = 4096;
        const DWORD             InBufferSize        = 4096;
        const DWORD             DefaultTimeoutMs    = 0;

        HANDLE hPipe = CreateNamedPipe( 
            transport.mPipeName.c_str(),
            PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
            PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
            MaxPipeInstances,
            OutBufferSize,
            InBufferSize,
            DefaultTimeoutMs,
            transport.mpSec);

        DWORD dwErr = GetLastError();

        RCF_VERIFY(hPipe != INVALID_HANDLE_VALUE, Exception(_RcfError_Pipe(), dwErr));

        mSocketPtr.reset( new AsioPipeHandle(ioService, hPipe) );
    }

    Win32NamedPipeSessionState::~Win32NamedPipeSessionState()
    {
    }

    const RemoteAddress & Win32NamedPipeSessionState::implGetRemoteAddress()
    {
        return mRemoteAddress;
    }

    void Win32NamedPipeSessionState::implRead(char * buffer, std::size_t bufferLen)
    {
        RCF_LOG_4()(bufferLen) 
            << "Win32NamedPipeSessionState - calling ReadFile().";

        ASIO_NS::windows::overlapped_ptr overlapped(
            mSocketPtr->get_io_service(), 
            ReadHandler(sharedFromThis()));

        DWORD dwBytesRead = 0;

        bool readOk = false;

        HANDLE hPipe = mSocketPtr->native();

        BOOL ok = ReadFile(
            hPipe,
            buffer,
            static_cast<DWORD>(bufferLen),
            &dwBytesRead,
            overlapped.get());

        DWORD realError = 0;
        DWORD dwErr = 0;

        if (!ok)
        {
            dwErr = GetLastError();

            if (    dwErr != ERROR_IO_PENDING 
                &&  dwErr != WSA_IO_PENDING 
                &&  dwErr != ERROR_MORE_DATA)
            {
                realError = dwErr;
            }
        }

        if (    dwBytesRead
            ||  (ok && dwBytesRead == 0 && bufferLen == 0)
            ||  (!ok && realError == 0))
        {
            readOk = true;
        }

        if (readOk)
        {
            overlapped.release();
        }
        else
        {
            AsioErrorCode ec(
                dwErr,
                ASIO_NS::error::get_system_category());

            overlapped.complete(ec, 0);
        }
    }

    void Win32NamedPipeSessionState::implWrite(const std::vector<ByteBuffer> & buffers)
    {
        RCF_LOG_4()(RCF::lengthByteBuffers(buffers))
            << "Win32NamedPipeSessionState - calling WriteFile().";

        ASIO_NS::windows::overlapped_ptr overlapped(
            mSocketPtr->get_io_service(), 
            WriteHandler(sharedFromThis()));

        const ByteBuffer & byteBuffer = buffers.front();

        DWORD dwBytesWritten = 0;

        bool writeOk = false;

        HANDLE hPipe = mSocketPtr->native();

        BOOL ok = WriteFile(
            hPipe,
            byteBuffer.getPtr(),
            static_cast<DWORD>(byteBuffer.getLength()),
            &dwBytesWritten,
            overlapped.get());

        DWORD dwErr = GetLastError();;

        if (!ok &&  (
            dwErr == ERROR_IO_PENDING 
            ||  dwErr == WSA_IO_PENDING 
            ||  dwErr == ERROR_MORE_DATA))
        {
            writeOk = true;
        }
        else if (dwBytesWritten)
        {
            writeOk = true;
        }

        if (writeOk)
        {
            overlapped.release();
        }
        else
        {
            AsioErrorCode ec(
                dwErr,
                ASIO_NS::error::get_system_category());

            overlapped.complete(ec, 0);
        }
    }

    void Win32NamedPipeSessionState::implWrite(AsioSessionState &toBeNotified, const char * buffer, std::size_t bufferLen)
    {
        ASIO_NS::windows::overlapped_ptr overlapped(
            mSocketPtr->get_io_service(), 
            boost::bind(
                &AsioSessionState::onNetworkWriteCompleted,
                toBeNotified.sharedFromThis(),
                ASIO_NS::placeholders::error,
                ASIO_NS::placeholders::bytes_transferred));

        DWORD dwBytesWritten = 0;

        bool writeOk = false;

        HANDLE hPipe = mSocketPtr->native();

        BOOL ok = WriteFile(
            hPipe,
            buffer,
            static_cast<DWORD>(bufferLen),
            &dwBytesWritten,
            overlapped.get());

        DWORD dwErr = GetLastError();

        if (!ok &&  (
            dwErr == ERROR_IO_PENDING 
            ||  dwErr == WSA_IO_PENDING 
            ||  dwErr == ERROR_MORE_DATA))
        {
            writeOk = true;
        }
        else if (dwBytesWritten)
        {
            writeOk = true;
        }

        if (writeOk)
        {
            overlapped.release();
        }
        else
        {
            AsioErrorCode ec(
                dwErr,
                ASIO_NS::error::get_system_category());

            overlapped.complete(ec, 0);
        }
    }

    void Win32NamedPipeSessionState::implAccept()
    {
        RCF_LOG_4()<< "Win32NamedPipeSessionState - calling ConnectNamedPipe().";

        ASIO_NS::windows::overlapped_ptr overlapped(
            mSocketPtr->get_io_service(), 
            boost::bind(
                &AsioSessionState::onAcceptCompleted,
                sharedFromThis(),
                ASIO_NS::placeholders::error));

        HANDLE hPipe = mSocketPtr->native();
        BOOL ok = ConnectNamedPipe(hPipe, overlapped.get());
        DWORD dwErr = GetLastError();

        // ConnectNamedPipe() can complete either synchronously or
        // asynchronously. We need to cater for both possibilities.

        if ( !ok && dwErr == ERROR_IO_PENDING )
        {
            overlapped.release();
        }
        else if (!ok && dwErr == ERROR_PIPE_CONNECTED)
        {
            AsioErrorCode ec;
            overlapped.complete(ec, 0);
        }
        else
        {
            // MSDN says ConectNamedPipe will always return 0, in overlapped mode.
            RCF_ASSERT(!ok);
        }
    }

    bool Win32NamedPipeSessionState::implOnAccept()
    {       
        return true;
    }

    bool Win32NamedPipeSessionState::implIsConnected()
    {
        //RCF_ASSERT(0 && "not implemented yet");
        return true;
    }

    void Win32NamedPipeSessionState::implClose()
    {
        mSocketPtr.reset();
    }

    ClientTransportAutoPtr Win32NamedPipeSessionState::implCreateClientTransport()
    {
        std::auto_ptr<Win32NamedPipeClientTransport> pipeClientTransportPtr(
            new Win32NamedPipeClientTransport(mSocketPtr, mRemotePipeName));

        return ClientTransportAutoPtr(pipeClientTransportPtr.release());
    }

    void Win32NamedPipeSessionState::implTransferNativeFrom(ClientTransport & clientTransport)
    {
        
        Win32NamedPipeClientTransport * pPipeClientTransport =
            dynamic_cast<Win32NamedPipeClientTransport *>(&clientTransport);

        if (pPipeClientTransport == NULL)
        {
            Exception e("incompatible client transport");
            RCF_THROW(e)(typeid(clientTransport));
        }

        Win32NamedPipeClientTransport & pipeClientTransport = *pPipeClientTransport;

        pipeClientTransport.associateWithIoService(mIoService);
        mSocketPtr = pipeClientTransport.releaseSocket();
        mRemotePipeName = pipeClientTransport.getPipeName();
    }

    void Win32NamedPipeSessionState::closeSocket(AsioPipeHandlePtr socketPtr)
    {
        socketPtr->close();
    }

    HANDLE Win32NamedPipeSessionState::getNativeHandle()
    {
        return mSocketPtr->native();
    }

    // Win32NamedPipeServerTransport

    tstring Win32NamedPipeServerTransport::getPipeName() const
    {
        return mPipeName;
    }

    Win32NamedPipeServerTransport::Win32NamedPipeServerTransport(
        const tstring & pipeName) :
            mPipeNameLock(INVALID_HANDLE_VALUE),
            mpSec(NULL)
    {
        if (pipeName.empty())
        {
            // Generate a pipe name dynamically, that won't conflict with any
            // other test executables that may be running concurrently.

            std::pair<tstring, HANDLE> pipeAndHandle = generateNewPipeName();
            mPipeName = pipeAndHandle.first + RCF_T("_0");
            mPipeNameLock = pipeAndHandle.second;
        }
        else
        {
            if (pipeName.at(0) == RCF_T('\\'))
            {
                mPipeName = pipeName;
            }
            else
            {
                mPipeName = RCF_T("\\\\.\\pipe\\") + pipeName;
            }
        }
    }

    Win32NamedPipeServerTransport::~Win32NamedPipeServerTransport()
    {
        if (mPipeNameLock != INVALID_HANDLE_VALUE)
        {
            CloseHandle(mPipeNameLock);
            mPipeNameLock = INVALID_HANDLE_VALUE;
        }
    }

    TransportType Win32NamedPipeServerTransport::getTransportType()
    {
        return Tt_Win32NamedPipe;
    }

    ServerTransportPtr Win32NamedPipeServerTransport::clone()
    {
        return ServerTransportPtr(new Win32NamedPipeServerTransport(mPipeName));
    }

    AsioSessionStatePtr Win32NamedPipeServerTransport::implCreateSessionState()
    {
        return AsioSessionStatePtr( new Win32NamedPipeSessionState(*this, *mpIoService) );
    }

    void Win32NamedPipeServerTransport::implOpen()
    {
    }

    ClientTransportAutoPtr Win32NamedPipeServerTransport::implCreateClientTransport(
        const Endpoint &endpoint)
    {
        const Win32NamedPipeEndpoint & pipeEndpoint = 
            dynamic_cast<const Win32NamedPipeEndpoint &>(endpoint);

        return pipeEndpoint.createClientTransport();
    }

    void Win32NamedPipeServerTransport::onServerStart(RcfServer & server)
    {
        AsioServerTransport::onServerStart(server);

        mpIoService = mTaskEntries[0].getThreadPool().getIoService();

        RCF_ASSERT(mAcceptorPtr.get() == NULL);

        if ( !mPipeName.empty() )
        {
            startAccepting();
        }

        RCF_LOG_2()(mPipeName) << "Win32NamedPipeServerTransport - listening on named pipe.";
    }

    void Win32NamedPipeServerTransport::onServerStop(RcfServer & server)
    {
        AsioServerTransport::onServerStop(server);
    }

    void Win32NamedPipeServerTransport::setSecurityAttributes(LPSECURITY_ATTRIBUTES pSec)
    {
        mpSec = pSec;
    }

    Win32NamedPipeImpersonator::Win32NamedPipeImpersonator() :
        mPipeSession( dynamic_cast<Win32NamedPipeSessionState &>(
            RCF::getTlsRcfSessionPtr()->getSessionState()))
    {
        impersonate();
    }

    Win32NamedPipeImpersonator::Win32NamedPipeImpersonator(Win32NamedPipeSessionState & session) :
        mPipeSession(session)
    {
        impersonate();
    }

    Win32NamedPipeImpersonator::~Win32NamedPipeImpersonator()
    {
        RCF_DTOR_BEGIN
            revertToSelf();
        RCF_DTOR_END
    }

    void Win32NamedPipeImpersonator::impersonate()
    {
        HANDLE hPipe = mPipeSession.mSocketPtr->native();
        BOOL ok = ImpersonateNamedPipeClient(hPipe);
        DWORD dwErr = GetLastError();
        RCF_VERIFY(ok, RCF::Exception(_RcfError_Pipe(), dwErr));
    }

    void Win32NamedPipeImpersonator::revertToSelf() const
    {
        BOOL ok = RevertToSelf();
        DWORD dwErr = GetLastError();
        RCF_VERIFY(ok, RCF::Exception(_RcfError_Pipe(), dwErr));
    }

    NullDacl::NullDacl()
    {
        ZeroMemory( &mSa, sizeof(mSa) );
        mSa.nLength = sizeof( mSa );
        mSa.bInheritHandle = FALSE;

        BOOL ok = InitializeSecurityDescriptor( &mSd, SECURITY_DESCRIPTOR_REVISION );
        DWORD dwErr = GetLastError();
        RCF_VERIFY(ok, Exception(_RcfError_Win32ApiError("InitializeSecurityDescriptor()"), dwErr));

        ok = SetSecurityDescriptorDacl( 
            &mSd,
            TRUE,
            (PACL) NULL,
            FALSE);

        dwErr = GetLastError();
        RCF_VERIFY(ok, Exception(_RcfError_Win32ApiError("SetSecurityDescriptorDacl()"), dwErr));

        mSa.lpSecurityDescriptor = &mSd;
    }

} // namespace RCF
