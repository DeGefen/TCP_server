#pragma once
#include "Config.h"
#include "HTTP_Status.h"
#include "HTTP_Request.h"
#include "FileManager.h"


class HTTP_Response {
public:
    explicit HTTP_Response(const HTTP_Request& request): request(request) {
        try {
            prepare();
        }
        catch (const HTTP_Status& e) {

        }
    }


    char* extract();

private:
    void prepare();
    string version;
    HTTP_Status status;
    map<string, string> headers;
    string body;
    HTTP_Request request;
};
