/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    responsehandler.cpp
 * @date    13.03.2013
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include "rpcprotocolclient.h"
#include "../common/jsonparser.h"

using namespace jsonrpc;

const utility::string_t RpcProtocolClient::KEY_PROTOCOL_VERSION = _U("jsonrpc");
const utility::string_t RpcProtocolClient::KEY_PROCEDURE_NAME   = _U("method");
const utility::string_t RpcProtocolClient::KEY_ID               = _U("id");
const utility::string_t RpcProtocolClient::KEY_PARAMETER        = _U("params");
const utility::string_t RpcProtocolClient::KEY_AUTH             = _U("auth");
const utility::string_t RpcProtocolClient::KEY_RESULT           = _U("result");
const utility::string_t RpcProtocolClient::KEY_ERROR            = _U("error");
const utility::string_t RpcProtocolClient::KEY_ERROR_CODE       = _U("code");
const utility::string_t RpcProtocolClient::KEY_ERROR_MESSAGE    = _U("message");

RpcProtocolClient::RpcProtocolClient(clientVersion_t version) : version(version)
{
}

utility::string_t RpcProtocolClient::BuildRequest(const utility::string_t &id,const utility::string_t& method, const json::value& parameter, bool isNotification)
{
	json::value request;
	this->BuildRequest(id, method, parameter, request, isNotification);
	return request.serialize();
}

void RpcProtocolClient::BuildRequest         (const utility::string_t &id,const utility::string_t &method, const json::value &parameter, utility::string_t &result, bool isNotification)
{
    json::value request;    
    this->BuildRequest(id, method,parameter,request, isNotification);
	result = request.serialize();
}

void RpcProtocolClient::HandleResponse       (const utility::string_t &response, json::value& result) throw(JsonRpcException)
{
	utility::stringstream_t ss;
	ss << response;	

	try
	{
		json::value value = json::value::parse(ss);
		this->HandleResponse(value, result);
	}
	catch (...)
	{
		throw JsonRpcException(Errors::ERROR_RPC_JSON_PARSE_ERROR, _U(" ") + response);
    }
}

utility::string_t RpcProtocolClient::HandleResponse(const json::value &value, json::value &result) throw(JsonRpcException)
{
    if(this->ValidateResponse(value))
    {
        if (this->HasError(value))
        {
            this->throwErrorException(value);
        }
        else
        {
			if(value.has_field(KEY_RESULT))				
			{
				result = value.at(KEY_RESULT);
			}
			else
			{
				result = json::value::null();
			}
        }
    }
    else
    {
		throw JsonRpcException(Errors::ERROR_CLIENT_INVALID_RESPONSE, _U(" ") + value.serialize());
    }

    return value.at(KEY_ID).as_string();
}

void RpcProtocolClient::BuildRequest         (const utility::string_t &id, const utility::string_t &method, const json::value &parameter, json::value &result, bool isNotification)
{
    if (this->version == JSONRPC_CLIENT_V2)
        result[KEY_PROTOCOL_VERSION] = json::value(_U("2.0"));
	result[KEY_PROCEDURE_NAME] = json::value(method);
    if (!parameter.is_null())
        result[KEY_PARAMETER] = parameter;
    if (!isNotification)
        result[KEY_ID] = json::value(id);
    else if (this->version == JSONRPC_CLIENT_V1)
		result[KEY_ID] = json::value::null();
}

void RpcProtocolClient::throwErrorException(const json::value &response)
{
	if (response.at(KEY_ERROR).has_field(KEY_ERROR_MESSAGE) && response.at(KEY_ERROR).at(KEY_ERROR_MESSAGE).is_string())
	{
		throw JsonRpcException(response.at(KEY_ERROR).at(KEY_ERROR_CODE).as_integer(), response.at(KEY_ERROR).at(KEY_ERROR_MESSAGE).as_string());
	}
	throw JsonRpcException(response.at(KEY_ERROR).at(KEY_ERROR_CODE).as_integer());
}

bool RpcProtocolClient::ValidateResponse(const json::value& response)
{
    if (!response.is_object() || !response.has_field(KEY_ID))
        return false;

    if (this->version == JSONRPC_CLIENT_V1)
    {
		if (!response.has_field(KEY_RESULT) || !response.has_field(KEY_ERROR))
            return false;
        if(!response.at(KEY_RESULT).is_null() && !response.at(KEY_ERROR).is_null())
            return false;
        if (!response.at(KEY_ERROR).is_null() && 
			!(response.at(KEY_ERROR).is_object() && 
			response.at(KEY_ERROR).has_field(KEY_ERROR_CODE) && 
			response.at(KEY_ERROR).at(KEY_ERROR_CODE).is_integer()))
            return false;
    }
    else if (this->version == JSONRPC_CLIENT_V2)
    {
		if (!response.has_field(KEY_PROTOCOL_VERSION) || response.at(KEY_PROTOCOL_VERSION).as_string() != _U("2.0"))
            return false;
		if (response.has_field(KEY_RESULT) && response.has_field(KEY_ERROR))
            return false;
		//if (!response.has_field(KEY_RESULT) && !response.has_field(KEY_ERROR))
        //    return false;
		if (response.has_field(KEY_ERROR) && 
			!(response.at(KEY_ERROR).is_object() && 
			response.at(KEY_ERROR).has_field(KEY_ERROR_CODE) && 
			response.at(KEY_ERROR).at(KEY_ERROR_CODE).is_integer()))
            return false;
    }

    return true;
}

bool RpcProtocolClient::HasError(const json::value& response)
{
    if (this->version == JSONRPC_CLIENT_V1 && !response.at(KEY_ERROR).is_null())
        return true;
	else if (this->version == JSONRPC_CLIENT_V2 && response.has_field(KEY_ERROR))
        return true;
    return false;
}
