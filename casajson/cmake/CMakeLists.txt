cmake_minimum_required(VERSION 2.6)
project(casajson)

SET(SRC_ROOT ${CMAKE_SOURCE_DIR}/../src )

SET (SRCS 
	${SRC_ROOT}/json/json.cpp
	${SRC_ROOT}/json/json_parsing.cpp
	${SRC_ROOT}/json/json_serialization.cpp
	${SRC_ROOT}/utilities/asyncrt_utils.cpp
	${SRC_ROOT}/pch/stdafx.cpp	
     )
	
IF(NOT BIN_PATH)
    SET(BIN_PATH ${CMAKE_SOURCE_DIR}/bin)
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
    #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	#SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS}  /MACHINE:X64" )
    #ENDIF()
ELSE()
   # SET(LIBS pthread dl boost_system boost_filesystem mpdec boost_thread boost_chrono pq boost_program_options jsoncpp)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wno-deprecated-declarations")
ENDIF()

ADD_DEFINITIONS( -DUSE_WIDECHAR -D_ASYNCRT_EXPORT )

INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/../include ${SRC_ROOT}/pch ${BOOST_ROOT})

#LINK_DIRECTORIES( ${LIB_DIRS} )

ADD_LIBRARY(casajson ${SRCS})

