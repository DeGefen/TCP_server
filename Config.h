#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _LIBCPP_NO_EXPERIMENTAL_DEPRECATION_WARNING_FILESYSTEM
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#include <cstring>
#include <ctime>

#include <experimental/filesystem>
#include <string>
#include <vector>
#include <map>
#include <vector>
#include <sstream>
#include <fstream>
#include <exception>

const int PORT = 27015;
const int MAX_SOCKETS = 60;
const int BUFFER_SIZE = 8 * 1024; // 8KB
