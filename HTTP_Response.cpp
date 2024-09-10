#include "HTTP_Response.h"

void HTTP_Response::prepare() {
    if (request.status.code != HTTP_Status::OK) {
        throw HTTP_Exception(request.status.code);
    }
    switch (request.method) {
        case HTTP_Request::GET:
            m_GET();
            break;
        case HTTP_Request::POST:
            if (!request.path.empty())
                invalidURL();
            cout << "Server [message received]: " << request.body << endl;
            break;
        case HTTP_Request::PUT:
            m_PUT();
            break;
        case HTTP_Request::DELETEE:
            m_DELETE();
            break;
        case HTTP_Request::TRACE:
            if (!request.path.empty())
                invalidURL();
            body = request.buffer;
            break;
        case HTTP_Request::HEAD:
            {
                m_GET();
                body.clear();
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


void HTTP_Response::m_GET() {
    if (request.path == RECORDS_PATH) {
        body = FileManager::list_records();
    }
    else if (request.path.substr(0, request.path.find('/') + 1) == PAGE_PATH)
    {
        formatPath();
        FileManager::read(request.path, body);
    }
    else invalidURL();
}

void HTTP_Response::m_PUT() {
    if (request.path.substr(0, request.path.find('/') + 1) != PAGE_PATH)
        invalidURL();
    formatPath();
    auto q = request.queryParams.find("mode");
    if (q == request.queryParams.end() || q->second == "overwrite")
        FileManager::write(request.path, request.body);
    else if (q->second == "append")
        FileManager::write(request.path, request.body, FileManager::APPEND);
    else
        throw HTTP_Exception(HTTP_Status::BAD_REQUEST, "Error: Unsupported mode");
}

void HTTP_Response::m_DELETE() {
    formatPath(ALLOW_DIRECTORIES);
    if (request.path.substr(0, request.path.find('/') + 1) != PAGE_PATH)
        invalidURL();
    FileManager::remove(request.path);
}

void HTTP_Response::formatPath(bool allowDir) {
    size_t numOfSlashes = count(request.path.begin(), request.path.end(), '/');
    size_t pos = request.path.find('.');

    if (numOfSlashes >= 3 || numOfSlashes == 0 || (numOfSlashes == 1 && pos != string::npos))
        invalidURL();

    if (numOfSlashes == 2) {
        if (pos != string::npos) {
            if (request.path.substr(pos + 1) != "txt")
                invalidURL();
        } else
            request.path += ".txt";
    }

    pos = request.path.find_last_of('/');
    if (pos == request.path.back())
        invalidURL();

    if (numOfSlashes == 1) {
        if (allowDir)
            return;
        else
            request.path += ".txt";
    }

    string subPath = request.path.substr(pos+ 1);
    if (subPath == "en.txt" || subPath == "he.txt" || subPath == "fr.txt")
        return;

    auto query = request.queryParams.find("lang");
    if (query == request.queryParams.end())
        query->second = "en";
    else if (query->second != "en" && query->second != "en" && query->second != "fr")
        throw HTTP_Exception(HTTP_Status::BAD_REQUEST, "Error: Unsupported language");
    pos = request.path.find_last_of('.');
    request.path.insert(pos, ('/' + query->second));
}



