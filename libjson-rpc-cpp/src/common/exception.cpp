/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    exception.cpp
 * @date    31.12.2012
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include "exception.h"
#include <boost/exception/get_error_info.hpp>
#include <boost/exception/info.hpp>

using namespace jsonrpc;

JsonRpcException::JsonRpcException(int code)
{
	JsonRpcExceptionInfo errinfo;

	errinfo.code = code;
	errinfo.message = Errors::GetErrorMessage(code);

	*this << info(errinfo);

}

JsonRpcException::JsonRpcException(int code, const std::wstring& message)
{
	JsonRpcExceptionInfo errinfo;

	errinfo.code = code;
	errinfo.message = message;

	*this << info(errinfo);
}

JsonRpcException::JsonRpcException(const std::wstring& message)
{
	JsonRpcExceptionInfo errinfo;

	errinfo.code = 0;
	errinfo.message = message;

	*this << info(errinfo);
}

JsonRpcException::~JsonRpcException() throw ()
{
}

int JsonRpcException::error_code() const
{
	JsonRpcExceptionInfo const* jsonErrorInfo = boost::get_error_info<info>(*this);
	return jsonErrorInfo->code;
}

const std::wstring& JsonRpcException::msg() const
{
	JsonRpcExceptionInfo const* jsonErrorInfo = boost::get_error_info<info>(*this);
	return jsonErrorInfo->message;
}

