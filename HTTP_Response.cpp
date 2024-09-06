#include "HTTP_Response.h"

void HTTP_Response::prepare() {
    if (request.status.code != HTTP_Status::OK) {
        throw HTTP_Exception(request.status.code);
    }
    switch (request.method) {
        case HTTP_Request::GET:
            {
                addExtension();
                FileManager::read(request.path.c_str() ,body);
            }
            break;
        case HTTP_Request::POST:
            cout << request.body;
            break;
        case HTTP_Request::PUT:
            {
                addExtension();
                auto q = request.queryParams.find("mode");
                if (q == request.queryParams.end() || q->second == "overwrite")
                    FileManager::write(request.path.c_str(), request.body);
                else if (q->second == "append")
                    FileManager::write(request.path.c_str(), request.body, FileManager::APPEND);
                else
                    throw HTTP_Exception(HTTP_Status::BAD_REQUEST, "Error: Unsupported mode");
            }
            break;
        case HTTP_Request::DELETE:
            if (!FileManager::remove(request.path.c_str()))
                throw HTTP_Exception(HTTP_Status::NO_CONTENT, "Info: File doesn't exists");
            break;
        case HTTP_Request::TRACE:
            body = request.buffer;
            break;
        case HTTP_Request::HEAD:
            {
                addExtension();
                string buffer;
                FileManager::read(request.path.c_str(), buffer);
            }
            break;
        case HTTP_Request::OPTIONS:

            break;
        default:
            throw HTTP_Exception(HTTP_Status::SERVICE_UNAVAILABLE, "Error: Service unavailable");
    }
}

string HTTP_Response::extract() {
    string response;
    response.append(request.version + ' ' + status.extract() + "\n");
    for (const auto& header: headers) {
        response.append(header.first + ": " + header.second + "\n");
    }
    response.append("\r\n");
    response.append(body);
    return response;
}

void HTTP_Response::addExtension() {
    auto s = request.path.find_last_of('.');
    if (s == string::npos)
        request.body.insert(s, ".html");
    else if (request.path.compare(s, 5, ".html") != 0 &&
             request.path.compare(s, 5, ".text") != 0 )
        throw HTTP_Exception(HTTP_Status::NOT_ACCEPTABLE, "Error: File type not supported");

    s = request.path.find_last_of('/');
    if (request.path.compare(s + 1, 3, "en.") == 0 ||
        request.path.compare(s + 1, 3, "he.") == 0 ||
        request.path.compare(s + 1, 3, "fr.") == 0) return;
    auto q = request.queryParams.find("lang");
    if (q == request.queryParams.end()) q->second = "en";
    else if (q->second != "en" && q->second != "en" && q->second != "fr")
        throw HTTP_Exception(HTTP_Status::BAD_REQUEST, "Error: Unsupported language");
    s = request.path.find_last_of('.');
    request.body.insert(s, ('/' + q->second));
}
