#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#include <string.h>
#include <time.h>

#include <string>
#include <vector>
#include <map>
#include <vector>
#include <sstream>

const int PORT = 27015;
const int MAX_SOCKETS = 60;
const int BUFFER_SIZE = 8 * 1024; // 8KB
