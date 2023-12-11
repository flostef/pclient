cmake_minimum_required(VERSION 3.6)
project(pclient LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall -fPIC -pipe -std=gnu90")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -fPIC -pipe -fexceptions")

# Find all the dependencies
find_package(OpenSSL REQUIRED)
set(Workflow_DIR "/Users/vlad/Projects/adf/srpc/workflow")
find_package(Workflow REQUIRED CONFIG HINTS ${Workflow_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

# Prefer to static link first
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a" ${CMAKE_FIND_LIBRARY_SUFFIXES})
find_library(Workflow_LIB workflow HINTS ${Workflow_DIR}/_lib)

# Set all the libraries here
set(LIB ${Workflow_LIB} pthread OpenSSL::SSL OpenSSL::Crypto)

# Add header directories and library directories here
include_directories(${OPENSSL_INCLUDE_DIR} ${WORKFLOW_INCLUDE_DIR})
link_directories(${OPENSSL_LINK_DIR} ${WORKFLOW_LIB_DIR})

add_subdirectory(src)

