cmake_minimum_required(VERSION 3.23)
project(casajson CXX)


SET(SRC_ROOT ${CMAKE_SOURCE_DIR}/src )
SET(INC_ROOT ${CMAKE_SOURCE_DIR}/include )

	
ADD_DEFINITIONS( -DUSE_WIDECHAR -D_ASYNCRT_EXPORT )

ADD_LIBRARY(casajson)

TARGET_SOURCES(casajson 
	PUBLIC
		${SRC_ROOT}/json/json.cpp
		${SRC_ROOT}/json/json_parsing.cpp
		${SRC_ROOT}/json/json_serialization.cpp
		${SRC_ROOT}/utilities/asyncrt_utils.cpp
		${SRC_ROOT}/pch/stdafx.cpp	
	PUBLIC 
	FILE_SET HEADERS 
		BASE_DIRS ${INC_ROOT}
		FILES
			${INC_ROOT}/casajson/json.h
			${INC_ROOT}/casajson/asyncrt_utils.h
			${INC_ROOT}/casajson/details/basic_types.h
			${INC_ROOT}/casajson/details/cpprest_compat.h
			${INC_ROOT}/casajson/details/nosal.h
   		)

#target_include_directories(casajson PUBLIC include)
target_include_directories(casajson PRIVATE ${SRC_ROOT}/pch)

#set_target_properties(casajson PROPERTIES PUBLIC_HEADER "include/casajson/json.h")

install(TARGETS casajson 
        FILE_SET HEADERS
		)
		
	

#install(TARGETS casajson 
#        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/casajson
#       )
