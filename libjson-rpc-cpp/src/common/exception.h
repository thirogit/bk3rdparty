/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    exception.h
 * @date    31.12.2012
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#ifndef JSONRPC_CPP_EXCEPTION_H_
#define JSONRPC_CPP_EXCEPTION_H_

#include <string>
#include <sstream>
#include <boost/exception/exception.hpp>
#include <boost/exception/error_info.hpp>

#include "errors.h"

namespace jsonrpc
{
    class JsonRpcException: public boost::exception
    {

		struct JsonRpcExceptionInfo
		{
			int code;
			std::wstring message;
		};

		struct tag_JsonRpcExceptionInfo;
		typedef boost::error_info<tag_JsonRpcExceptionInfo, JsonRpcExceptionInfo> info;

        public:
            JsonRpcException(int code);

            JsonRpcException(int code, const std::wstring& message);

            JsonRpcException(const std::wstring& message);

            virtual ~JsonRpcException() throw ();

            int error_code() const;

            const std::wstring& msg() const;			           

        private:
            
    };

} /* namespace jsonrpc */
#endif /* JSONRPC_CPP_EXCEPTION_H_ */
