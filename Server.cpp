#include "Config.h"
#include "NetworkManager.h"

/*
 * TODO:: [x] CHECK:: <x> FileManager,
 *                    <x> HTTP_Request,
 *                    <x> HTTP_Response:: (x) GET
 *                                        (x) POST
 *                                        (x) PUT
 *                                        (x) DELETE
 *                                        (x) TRACE
 *                                        (x) HEAD
 *                                        (x) OPTIONS
 *
 * TODO:: [X] ADD:: <x> Connection timeout,
 *                  <x> Response headers
 *
 */

int main()
{
    FileManager::makePath("pages/file1/en.txt");
    FileManager::write("pages/file1/en.txt", "hello");
    NetworkManager server;
    server.run();
}