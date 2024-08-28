#pragma once
#include "config.h"

class HTTPRequest {
    string method;
    string url;
    string protocol;
    map<string, string> queryParams;
    map<string, string> headers;
    string body;

public:
    void extract(const char* buffer);
};

