#include "Config.h"
//#include "NetworkManager.h"
#include "HTTP_Response.h"

/*
 * TODO:: [ ] CHECK:: <x> FileManager,
 *                    <x> HTTP_Request,
 *                    < > HTTP_Response:: (x) GET
 *                                        (x) POST
 *                                        (x) PUT
 *                                        (x) DELETE
 *                                        (x) TRACE
 *                                        (x) HEAD
 *                                        ( ) OPTIONS
 *
 * TODO:: [X] ADD:: <x> Connection timeout,
 *                  <x> Response headers
 *
 */

int main()
{
    string recvHTTP;
    FileManager::read("http.txt", recvHTTP);
    HTTP_Request request;
    request.insert(recvHTTP.c_str());
    HTTP_Response response(request);
    cout << response.extract();
}