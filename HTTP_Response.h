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
        catch (const HTTP_Exception& e) {
            status = e.code;
            body = e.msg;
        }
    }

    string extract();

private:
    static constexpr auto PAGE_PATH = "/pages/";
    static constexpr auto RECORDS_PATH = "/records";

    void addExtension();
    void prepare();
    void get();
    void put();
    void remove();
    string version;
    HTTP_Status status;
    map<string, string> headers;
    string body;
    HTTP_Request request;
};
