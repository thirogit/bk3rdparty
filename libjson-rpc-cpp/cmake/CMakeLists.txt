cmake_minimum_required(VERSION 2.6)
project(libjson-rpc-cpp)

SET(SRC_ROOT ${CMAKE_SOURCE_DIR}/../src )

SET (SRCS ${SRC_ROOT}/client/client.cpp
	${SRC_ROOT}/client/rpcprotocolclient.cpp
	${SRC_ROOT}/common/errors.cpp
	${SRC_ROOT}/common/exception.cpp
	${SRC_ROOT}/common/messageproperties.cpp
     )
	
IF(NOT BIN_PATH)
    SET(BIN_PATH ${CMAKE_SOURCE_DIR}/bin)
ENDIF() 

IF(NOT BK_3RDPARTY_ROOT)
    SET(BK_3RDPARTY_ROOT ${CMAKE_SOURCE_DIR}/../../ )
ENDIF()

IF(NOT CASAJSON_ROOT)
    SET(CASAJSON_ROOT ${BK_3RDPARTY_ROOT}/casajson )
ENDIF()

IF(NOT BOOST_ROOT)
    SET(BOOST_ROOT /usr/include )
ENDIF()


SET(LIBRARY_OUTPUT_PATH ${BIN_PATH})

LINK_DIRECTORIES( ${BIN_PATH} )

	
#SET(LIB_DIRS )
    
IF(MINGW)
    #SET(LIBS ws2_32 mswsock crypt32)
ELSEIF(MSVC)
    #SET(LIBS ws2_32)
    #IF(CMAKE_CL_64)
    #    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
    #ENDIF()
ELSE()
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wno-deprecated-declarations")
ENDIF()


ADD_DEFINITIONS( -DUSE_WIDECHAR )

INCLUDE_DIRECTORIES( ${CASAJSON_ROOT}/include ${BOOST_ROOT} )

#LINK_DIRECTORIES( ${LIB_DIRS} )

ADD_LIBRARY(jsonrpc ${SRCS})



