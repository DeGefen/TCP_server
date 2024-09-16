#pragma once
#include "Config.h"
#include "HTTP_Status.h"
#include "HTTP_Request.h"
#include "FileManager.h"

// HTTP Response class to create and manage responses for each request
class HTTP_Response {
public:
    // INIT
    explicit HTTP_Response(const HTTP_Request& request): request(request) {
        try {
            prepare();
        }
        catch (const HTTP_Exception& e) {
            status = e.code;
            body = e.msg;
        }
        catch (const exception& e) {
            status = HTTP_Status::INTERNAL_SERVER_ERROR;
            body = e.what();
        }
        catch (...) {
            status = HTTP_Status::INTERNAL_SERVER_ERROR;
            body = "Error: Unknow error";
        }
    }

    // Builds the full HTTP response string, including headers and body.
    string extract();

private:
    static constexpr auto PAGE_PATH = "pages/";
    static constexpr auto RECORDS_PATH = "records";
    static constexpr auto ALLOW_DIRECTORIES = true;
    static constexpr auto FILES_ONLY = false;

    // Formats and validates the request path.
    // Adds file extensions or directories when needed.
    void formatPath(bool allowDir = FILES_ONLY);

    // Prepares the HTTP response based on the request method and status.
    // Throws an exception if the request status is not OK.
    void prepare();

    // Handles GET requests. Reads file content or lists files.
    void m_GET();

    // Handles PUT requests. Writes or appends content to files.
    void m_PUT();

    // Handles DELETE requests. Deletes the specified file.
    void m_DELETE();

    static void invalidURL() {
        throw HTTP_Exception(HTTP_Status::BAD_REQUEST, "Error: Invalid URL");
    }

    string version = "http/1.1";
    HTTP_Status status;
    map<string, string> headers;
    string body;
    HTTP_Request request;
};
