#include "HTTP_Request.h"

void HTTP_Request::insert(const char* recvBuffer) {
    // reset objects
    try {
        buffer = recvBuffer;
        queryParams.clear();
        headers.clear();

        string line, p1, p2;
        istringstream requestStream(buffer);

        getline(requestStream, line);
        istringstream lineStream(line);
        lineStream >> p1;
        method = methods.at(p1);

        lineStream >> p1 >> version;
        if (version != "HTTP/1.1") {
            throw HTTP_Exception(HTTP_Status::HTTP_VERSION_NOT_SUPPORTED);
        }

        istringstream urlStream(p1);
        getline(urlStream, path, '?');
        while (getline(urlStream, p1, '=')) {
            getline(urlStream, p2, '&');
            queryParams[p1] = p2;
        }

        while (requestStream.peek() != '\n' && getline(requestStream, line)) {
            size_t colonPos = line.find(':');
            p1 = line.substr(0, colonPos);
            p2 = line.substr(colonPos + 2);
            headers[p1] = p2;
        }

        getline(requestStream, body, '\0');
    }
    catch (const out_of_range& e) {
        status = HTTP_Status::METHOD_NOT_ALLOWED;
    }
    catch (const HTTP_Exception& e) {
        status = e.code;
    }
    catch (exception& e) {
        status = HTTP_Status::INTERNAL_SERVER_ERROR;
    }
}




