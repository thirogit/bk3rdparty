/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    client.cpp
 * @date    03.01.2013
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include "client.h"
#include "rpcprotocolclient.h"

using namespace jsonrpc;

Client::Client(IClientConnector *connector, clientVersion_t version) :
    connector(connector)
{
    this->protocol = new RpcProtocolClient(version);
}

Client::~Client()
{
    delete this->protocol;
}

void Client::CallMethod(const utility::string_t &id,const utility::string_t &name, const json::value &parameter, json::value& result, const RPCMessageProperties& msgProperties) throw(JsonRpcException)
{
    utility::string_t request;
    protocol->BuildRequest(id,name, parameter, request, false);
	utility::string_t response = connector->SendRPCMessage(request, msgProperties);
    protocol->HandleResponse(response, result);
}

json::value Client::CallMethod(const utility::string_t &id,const utility::string_t& name, const json::value& parameter) throw(JsonRpcException)
{
    json::value result;
    this->CallMethod(id,name, parameter, result, RPCMessageProperties());
    return result;
}

json::value Client::CallMethod(const utility::string_t &id,const utility::string_t &name, const json::value &parameters, const RPCMessageProperties& msgProperties) throw (JsonRpcException)
{
	json::value result;
	this->CallMethod(id,name, parameters, result, msgProperties);
	return result;
}


void Client::CallNotification(const utility::string_t &id,const utility::string_t& name, const json::value& parameters) throw(JsonRpcException)
{
    utility::string_t request, response;
    protocol->BuildRequest(id,name, parameters, request, true);
    connector->SendRPCMessage(request,RPCMessageProperties());
}
