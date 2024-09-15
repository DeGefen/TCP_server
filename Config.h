#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _LIBCPP_NO_EXPERIMENTAL_DEPRECATION_WARNING_FILESYSTEM
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>

#include <cstring>
#include <ctime>
#include <experimental/filesystem>
#include <sstream>
#include <fstream>
#include <exception>
//#include <unistd.h>
#include <climits>

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <list>

using namespace std;
namespace fs = std::experimental::filesystem;

const int PORT = 27015;
const int MAX_SOCKETS = 60;
const int BUFFER_SIZE = 8 * 1024; // 8KB

//#define _DEBUGING_SERVER
