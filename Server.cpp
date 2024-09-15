#include "Config.h"
#include "NetworkManager.h"

/*
 * TODO:: [ ] CHECK:: <x> FileManager,
 *                    <x> HTTP_Request,
 *                    <x> HTTP_Response:: (x) GET
 *                                        (x) POST
 *                                        (x) PUT
 *                                        (x) DELETE
 *                                        (x) TRACE
 *                                        (x) HEAD
 *                                        (x) OPTIONS
 *                    <x> server path recognition
 *
 * TODO:: [X] ADD:: <x> Connection timeout,
 *                  <x> Response headers
 *                  < > error handling for when server sending informtion

 * TODO:: [X] REFRACTOR:: <x> paths to C:\temp\pages
 *
 */

int main() {
    NetworkManager server;
    server.run();
}