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

struct SocketState
{
    SOCKET id;			// Socket handle
    int	recv;			// Receiving?
    int	send;			// Sending?
    int sendSubType;	// Sending sub-type
    char buffer[128];
    int len;
};

const int TIME_PORT = 27015;
const int MAX_SOCKETS = 60;
const int EMPTY = 0;
const int LISTEN  = 1;
const int RECEIVE = 2;
const int IDLE = 3;
const int SEND = 4;
const int SEND_TIME = 1;
const int SEND_SECONDS = 2;
