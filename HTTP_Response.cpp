#include "HTTP_Response.h"

void HTTP_Response::prepare() {
    if (request.status.code != HTTP_Status::OK) {
        throw HTTP_Exception(request.status.code);
    }
    switch (request.method) {
        case HTTP_Request::GET:     // TODO: lang query
            FileManager::read(request.path.c_str() ,body);
            break;
        case HTTP_Request::POST:

            break;
        case HTTP_Request::PUT:
            FileManager::write(request.path.c_str(), request.body);
            break;
        case HTTP_Request::DELETE:
            if (!FileManager::remove(request.path.c_str()))
                status = HTTP_Status::NO_CONTENT;
            break;
        case HTTP_Request::TRACE:
            for (auto & header : request.headers) {
                body += header.first + ": " + header.second + "\r\n";
            }
            break;
        case HTTP_Request::HEAD:
            {
                string buffer;
                FileManager::read(request.path.c_str(), buffer);
            }
            break;
        case HTTP_Request::OPTIONS:

            break;
        default:
            throw HTTP_Exception(HTTP_Status::SERVICE_UNAVAILABLE);
    }
}

char* HTTP_Response::extract() {

}