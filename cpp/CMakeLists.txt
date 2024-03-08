cmake_minimum_required(VERSION 3.8.11)
project("Hesai SDK Wrapper" LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(VERSION_MAJOR 2)
set(VERSION_MINOR 0)
set(VERSION_TINY  5)
# configure_file(
#   "${CMAKE_CURRENT_SOURCE_DIR}/HesaiLidar_SDK_2.0/Version.h.in"
#   "${CMAKE_CURRENT_BINARY_DIR}/Version.h"
# )

set(TARGET hesai-lidar-sdk)

find_package(Boost REQUIRED COMPONENTS system filesystem thread)
find_package(OpenSSL REQUIRED)
add_definitions(-DQT_MESSAGELOGCONTEXT)

add_subdirectory(HesaiLidar_SDK_2.0/libhesai)

# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/HesaiLidar_SDK_2.0)

add_library(${TARGET} STATIC
    # hesai_lidar_sdk.cpp 
    wrapper/w_lidar.cpp
)

target_include_directories(${TARGET}
    PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/wrapper
    ${CMAKE_CURRENT_SOURCE_DIR}/HesaiLidar_SDK_2.0/libhesai
    ${CMAKE_CURRENT_SOURCE_DIR}/HesaiLidar_SDK_2.0/libhesai/Lidar
    ${CMAKE_CURRENT_SOURCE_DIR}/HesaiLidar_SDK_2.0/libhesai/UdpParser
    ${CMAKE_CURRENT_SOURCE_DIR}/HesaiLidar_SDK_2.0/libhesai/UdpParser/include
    ${CMAKE_CURRENT_SOURCE_DIR}/HesaiLidar_SDK_2.0/libhesai/UdpParser/src
    ${CMAKE_CURRENT_SOURCE_DIR}/HesaiLidar_SDK_2.0/libhesai/UdpProtocol
    ${CMAKE_CURRENT_SOURCE_DIR}/HesaiLidar_SDK_2.0/libhesai/Source/include
    ${CMAKE_CURRENT_SOURCE_DIR}/HesaiLidar_SDK_2.0/libhesai/Container/include
    ${CMAKE_CURRENT_SOURCE_DIR}/HesaiLidar_SDK_2.0/libhesai/Container/src
    ${CMAKE_CURRENT_SOURCE_DIR}/HesaiLidar_SDK_2.0/libhesai/PtcClient/include
    ${CMAKE_CURRENT_SOURCE_DIR}/HesaiLidar_SDK_2.0/libhesai/PtcParser
    ${CMAKE_CURRENT_SOURCE_DIR}/HesaiLidar_SDK_2.0/libhesai/PtcParser/include
    ${CMAKE_CURRENT_SOURCE_DIR}/HesaiLidar_SDK_2.0/libhesai/PtcParser/src
    ${CMAKE_CURRENT_SOURCE_DIR}/HesaiLidar_SDK_2.0/libhesai/Logger/include
    ${CMAKE_CURRENT_SOURCE_DIR}/HesaiLidar_SDK_2.0/libhesai/include
    ${CMAKE_CURRENT_SOURCE_DIR}/HesaiLidar_SDK_2.0/driver
    ${Boost_INCLUDE_DIRS}
    ${OPENSSL_INCLUDE_DIR}
)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/HesaiLidar_SDK_2.0/libhesai/PtcClient/lib)
link_directories(${Boost_LIBRARY_DIRS})

target_link_libraries(${TARGET}
    ${Boost_LIBRARIES}
    source_lib
    container_lib
    ptcClient_lib
    ptcParser_lib
    log_lib
    ${OPENSSL_LIBRARIES}
)

install(TARGETS 
    ${TARGET} 
    source_lib 
    container_lib 
    ptcClient_lib 
    ptcParser_lib 
    log_lib 
    platutils_lib
    DESTINATION .)
