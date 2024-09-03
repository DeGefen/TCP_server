#include "HTTP_Request.h"

void HTTP_Request::insert(const char* buffer) {
    // reset objects
    queryParams.clear();
    headers.clear();

    string line, p1, p2;
    istringstream requestStream(buffer);

    getline(requestStream, line);
    istringstream lineStream(line);
    lineStream >> p1;
    method = to_methods[p1];

    lineStream >> p1 >> version;
    istringstream urlStream(p1);
    getline(urlStream, url, '?');
    while (getline(urlStream, p1, '='))
    {
        getline(urlStream, p2, '&');
        queryParams[p1] = p2;
    }

    while (requestStream.peek() != '\n' && getline(requestStream, line))
    {
        size_t colonPos = line.find(':');
        p1 = line.substr(0, colonPos);
        p1 = line.substr(colonPos+2);
        headers[p1] = p2;
    }

    getline(requestStream, body, '\0');
}

