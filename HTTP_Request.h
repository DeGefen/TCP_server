#pragma once
#include "config.h"

class HTTP_Request {
public:
    enum Methods { GET, PUT, POST, DELETE, PATCH, HEAD, OPTIONS, TRACE };
    map<string, Methods> to_methods {{"GET",     Methods::GET}, {"PUT", Methods::PUT},
                                     {"POST",    Methods::POST}, {"DELETE", Methods::DELETE},
                                     {"PATCH",   Methods::PATCH}, {"HEAD", Methods::HEAD},
                                     {"OPTIONS", Methods::OPTIONS}, {"TRACE", Methods::TRACE} };
    void insert(const char* buffer);

private:
    Methods method;
    string url;
    string version;
    map<string, string> queryParams;
    map<string, string> headers;
    string body;
};