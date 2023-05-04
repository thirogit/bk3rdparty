#ifndef JSONRPC_CPP_CLIENTCONNECTOR_H_
#define JSONRPC_CPP_CLIENTCONNECTOR_H_

#include <string>
#include "../common/exception.h"
#include "../common/messageproperties.h"

namespace jsonrpc
{
    class IClientConnector
    {
        public:
            virtual ~IClientConnector(){}

			virtual utility::string_t SendRPCMessage(const utility::string_t& message,const RPCMessageProperties& properties) throw(JsonRpcException) = 0;
    };
} /* namespace jsonrpc */
#endif /* JSONRPC_CPP_CLIENTCONNECTOR_H_ */
