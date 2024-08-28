#include "HTTPRequest.h"

void HTTPRequest::extract(const char* buffer) {
    // clear map objects
    queryParams.clear();
    headers.clear();

    istringstream requestStream(buffer);
    string line, param1, param2;

    getline(requestStream, line);
    istringstream lineStream(line);
    lineStream >> method >> param1 >> protocol;
    istringstream urlStream(param1);
    getline(urlStream, url, '?');
    while (getline(urlStream, param1, '='))
    {
        getline(urlStream, param2, '&');
        queryParams[param1] = param2;
    }

    while (requestStream.peek() != '\n' && getline(requestStream, line))
    {
        size_t colonPos = line.find(':');
        param1 = line.substr(0, colonPos);
        param2 = line.substr(colonPos+2);
        headers[param1] = param2;
    }

    getline(requestStream, body, '\0');
}

