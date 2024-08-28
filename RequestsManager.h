#pragma once
#include "config.h"

class RequestManger {
    string method;
    string url;
    string httpVersion;
    map<string, string> queryParams;
    map<string, string> headers;
    string body;

    void extract(char* recvBuffer);
};

