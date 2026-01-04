/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    client.h
 * @date    03.01.2013
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#ifndef JSONRPC_CPP_CLIENT_H_
#define JSONRPC_CPP_CLIENT_H_

#include "iclientconnector.h"
#include "../common/jsonparser.h"

#include <vector>
#include <map>

namespace jsonrpc
{
    class RpcProtocolClient;

    typedef enum {JSONRPC_CLIENT_V1, JSONRPC_CLIENT_V2} clientVersion_t;

    class Client
    {
        public:
            Client(IClientConnector *connector, clientVersion_t version = JSONRPC_CLIENT_V2);
            virtual ~Client();

			void        CallMethod(const utility::string_t &id,const utility::string_t &name, const json::value &parameter, json::value& result, const RPCMessageProperties& msgProperties) throw (JsonRpcException);

			json::value CallMethod(const utility::string_t &id,const utility::string_t &name, const json::value &parameter) throw (JsonRpcException);
			json::value CallMethod(const utility::string_t &id,const utility::string_t &name, const json::value &parameters,const RPCMessageProperties& msgProperties) throw (JsonRpcException);

			void        CallNotification(const utility::string_t &id,const utility::string_t& name, const json::value& parameters) throw (JsonRpcException);

        private:
           IClientConnector  *connector;
           RpcProtocolClient *protocol;

    };

} /* namespace jsonrpc */
#endif /* JSONRPC_CPP_CLIENT_H_ */
