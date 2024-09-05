cmake_minimum_required(VERSION 3.24)
project(TCP_server)

set(CMAKE_CXX_STANDARD 17)

add_library(TCP_server HTTP_Request.cpp Server.cpp Config.h NetworkManager.cpp NetworkManager.h
        HTTP_Response.cpp
        HTTP_Response.h
        FileManager.cpp
        FileManager.h
        HTTP_Status.h)
