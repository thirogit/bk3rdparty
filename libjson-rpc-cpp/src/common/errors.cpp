/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    errors.cpp
 * @date    31.12.2012
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include "errors.h"
#include "exception.h"

using namespace jsonrpc;

std::map<int, std::wstring> Errors::possibleErrors;
Errors::_init Errors::_initializer;

const int Errors::ERROR_RPC_JSON_PARSE_ERROR =  -32700;
const int Errors::ERROR_RPC_METHOD_NOT_FOUND =  -32601;
const int Errors::ERROR_RPC_INVALID_REQUEST =   -32600;
const int Errors::ERROR_RPC_INVALID_PARAMS =    -32602;
const int Errors::ERROR_RPC_INTERNAL_ERROR =    -32603;

const int Errors::ERROR_SERVER_PROCEDURE_IS_METHOD =                  -32604;
const int Errors::ERROR_SERVER_PROCEDURE_IS_NOTIFICATION =            -32605;
const int Errors::ERROR_SERVER_PROCEDURE_POINTER_IS_NULL =            -32606;
const int Errors::ERROR_SERVER_PROCEDURE_SPECIFICATION_NOT_FOUND =    -32000;
const int Errors::ERROR_SERVER_CONNECTOR =                            -32002;
const int Errors::ERROR_SERVER_PROCEDURE_SPECIFICATION_SYNTAX =       -32007;

const int Errors::ERROR_CLIENT_CONNECTOR =   -32003;
const int Errors::ERROR_CLIENT_INVALID_RESPONSE =     -32001;

Errors::_init::_init() {
    //Official Errors
    possibleErrors[ERROR_RPC_INVALID_REQUEST] =
            L"INVALID_JSON_REQUEST: The JSON sent is not a valid JSON-RPC Request object";
    possibleErrors[ERROR_RPC_METHOD_NOT_FOUND] =
            L"METHOD_NOT_FOUND: The method being requested is not available on this server";
    possibleErrors[ERROR_RPC_INVALID_PARAMS] =
            L"INVALID_PARAMS: Invalid method parameters (invalid name and/or type) recognised";
    possibleErrors[ERROR_RPC_JSON_PARSE_ERROR] =
            L"JSON_PARSE_ERROR: The JSON-Object is not JSON-Valid";
    possibleErrors[ERROR_RPC_INTERNAL_ERROR] = L"INTERNAL_ERROR: ";

    possibleErrors[ERROR_SERVER_PROCEDURE_IS_METHOD] =
            L"PROCEDURE_IS_METHOD: The requested notification is declared as a method";
    possibleErrors[ERROR_SERVER_PROCEDURE_IS_NOTIFICATION] =
            L"PROCEDURE_IS_NOTIFICATION: The requested method is declared as notification";
    possibleErrors[ERROR_SERVER_PROCEDURE_POINTER_IS_NULL] =
            L"PROCEDURE_POINTER_IS_NULL: Server has no function Reference registered";
    possibleErrors[ERROR_SERVER_PROCEDURE_SPECIFICATION_NOT_FOUND] = L"Configuration file was not found";

    possibleErrors[ERROR_SERVER_PROCEDURE_SPECIFICATION_SYNTAX] = L"Procedure specification mallformed";

    possibleErrors[ERROR_CLIENT_INVALID_RESPONSE] = L"The response is invalid";
    possibleErrors[ERROR_CLIENT_CONNECTOR] = L"Client connector error";
    possibleErrors[ERROR_SERVER_CONNECTOR] = L"Server connector error";
}

std::wstring Errors::GetErrorMessage(int errorCode)
{
    if(possibleErrors.find(errorCode) == possibleErrors.end())
    {
        return L"";
    }
    return possibleErrors[errorCode];
}
