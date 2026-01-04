/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    responsehandler.h
 * @date    13.03.2013
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#ifndef RESPONSEHANDLER_H
#define RESPONSEHANDLER_H

#include "../common/exception.h"
#include "../common/jsonparser.h"
#include <string>
#include "client.h"

namespace jsonrpc {


    /**
     * @brief The RpcProtocolClient class handles the json-rpc 2.0 protocol for the client side.
     */
    class RpcProtocolClient
    {
        public:
            RpcProtocolClient(clientVersion_t version = JSONRPC_CLIENT_V2);

            /**
             * @brief This method builds a valid json-rpc 2.0 request object based on passed parameters.
             * The id starts at 1 and is incremented for each request. To reset this value to one, call
             * the jsonrpc::RpcProRpcProtocolClient::resetId() method.
             * @param method - name of method or notification to be called
             * @param parameter - parameters represented as json objects
             * @return the string representation of the request to be built.
             */
			utility::string_t BuildRequest(const utility::string_t &id,const utility::string_t& method, const json::value& parameter, bool isNotification);

            /**
             * @brief BuildRequest does the same as utility::string_t jsonrpc::RpcProRpcProtocolClient::BuildRequest(const utility::string_t& method, const Json::Value& parameter);
             * The only difference here is that the result is returend by value, using the result parameter.
             * @param method - name of method or notification to be called
             * @param parameter - parameters represented as json objects
             * @param result - the string representation will be hold within this reference.
             */
			void BuildRequest(const utility::string_t &id,const utility::string_t& method, const json::value& parameter, utility::string_t& result, bool isNotification);


            /**
             * @brief Does the same as json::value RpcProtocolClient::HandleResponse(const utility::string_t& response) throw(Exception)
             * but returns result as reference for performance speed up.
             */
			void HandleResponse(const utility::string_t &response, json::value &result) throw (JsonRpcException);

            /**
             * @brief HandleResponse
             * @param response
             * @param result
             * @return response id
             */
			utility::string_t HandleResponse(const json::value &response, json::value &result) throw (JsonRpcException);

            static const utility::string_t KEY_PROTOCOL_VERSION;
            static const utility::string_t KEY_PROCEDURE_NAME;
            static const utility::string_t KEY_ID;
            static const utility::string_t KEY_PARAMETER;
            static const utility::string_t KEY_AUTH;
            static const utility::string_t KEY_RESULT;
            static const utility::string_t KEY_ERROR;
            static const utility::string_t KEY_ERROR_CODE;
            static const utility::string_t KEY_ERROR_MESSAGE;

        private:
            clientVersion_t version;

            void BuildRequest(const utility::string_t &id, const utility::string_t& method, const json::value& parameter, json::value& result, bool isNotification);
            bool ValidateResponse(const json::value &response);
            bool HasError(const json::value &response);
            void throwErrorException(const json::value &response);
    };
}
#endif // RESPONSEHANDLER_H
