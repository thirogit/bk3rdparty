cmake_minimum_required(VERSION 3.23)
project(jsonrpc CXX)

find_package(casajson)


SET(SRC_ROOT ${CMAKE_SOURCE_DIR}/src )
SET(INC_ROOT ${CMAKE_SOURCE_DIR}/include )
	
ADD_DEFINITIONS( -DUSE_WIDECHAR )

ADD_LIBRARY(jsonrpc)

include_directories(${casajson_INCLUDE_DIRS} ${INC_ROOT}/client ${INC_ROOT}/common) 

TARGET_SOURCES(jsonrpc 
	PUBLIC
		${SRC_ROOT}/client/client.cpp
		${SRC_ROOT}/client/rpcprotocolclient.cpp
		${SRC_ROOT}/common/errors.cpp
		${SRC_ROOT}/common/exception.cpp
		${SRC_ROOT}/common/messageproperties.cpp
   		)

install(FILES  
				${INC_ROOT}/jsonrpcclient.h
				${INC_ROOT}/jsonrpcserver.h 
		DESTINATION  
				${CMAKE_INSTALL_INCLUDEDIR} 
		)
		
install(DIRECTORY  ${INC_ROOT}/common DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} FILES_MATCHING PATTERN "*h")
install(DIRECTORY  ${INC_ROOT}/client DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} FILES_MATCHING PATTERN "*h")
install(TARGETS jsonrpc )
		
	


