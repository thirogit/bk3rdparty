cmake_minimum_required(VERSION 3.23)
project(PackageTest CXX)

find_package(casajson CONFIG REQUIRED)
find_package(Boost)

ADD_DEFINITIONS( -DUSE_WIDECHAR -D_ASYNCRT_EXPORT )

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 
endif()

add_executable(example src/example.cpp)
target_link_libraries(example casajson::casajson)
