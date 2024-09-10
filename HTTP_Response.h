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
//        catch (const exception& e) {      TODO: REMOVE BEFORE FLIGHT
//            status = HTTP_Status::INTERNAL_SERVER_ERROR;
//            body = e.what();
//        }
    }

    string extract();

private:
    static constexpr auto PAGE_PATH = "pages/";
    static constexpr auto RECORDS_PATH = "records";
    static constexpr auto ALLOW_DIRECTORIES = true;
    static constexpr auto FILES_ONLY = false;

    void formatPath(bool allowDir = FILES_ONLY);
    void prepare();
    void m_GET();
    void m_PUT();
    void m_DELETE();

    static void invalidURL() {
        throw HTTP_Exception(HTTP_Status::BAD_REQUEST, "Error: Invalid URL");
    }

    string version;
    HTTP_Status status;
    map<string, string> headers;
    string body;
    HTTP_Request request;
};
