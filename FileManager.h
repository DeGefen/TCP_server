#pragma once
#include "config.h"
#include "HTTP_Response.h"

class FileManager {
public:
    static void read(const char* path, string& body);
    static void write(const char* path, char* body);
    static void append(const char* path, char* body);
    static void remove(const char* path);
    static void makePath(const char* path);

private:
    static void exists(const char* path) {
        if (!filesystem::exists(path)) {
            throw HTTP_Status(HTTP_Status::NOT_FOUND);
        }
    }

    static void fail(fstream& file) {
        if (file.fail()) {
            throw HTTP_Status(HTTP_Status::INTERNAL_SERVER_ERROR);
        }
    }
};