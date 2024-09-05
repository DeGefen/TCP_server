#pragma once
#include "Config.h"
#include "HTTP_Status.h"

class HTTP_Response;

class HTTP_Request {
public:
    enum Method { GET, PUT, POST, DELETE, PATCH, HEAD, OPTIONS, TRACE };
    const map<string, Method> methods {{"GET",     Method::GET}, {"PUT", Method::PUT},
                                 {"POST",    Method::POST}, {"DELETE", Method::DELETE},
                                 {"PATCH",   Method::PATCH}, {"HEAD", Method::HEAD},
                                 {"OPTIONS", Method::OPTIONS}, {"TRACE", Method::TRACE} };
    void insert(const char* buffer);

private:
    Method method;
    string path;
    string version;
    map<string, string> queryParams;
    map<string, string> headers;
    string body;
    HTTP_Status status;

    friend HTTP_Response;
};