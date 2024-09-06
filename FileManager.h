#pragma once
#include "Config.h"
#include "HTTP_Status.h"

class FileManager {
public:
    static constexpr bool APPEND = true;
    static constexpr bool OVERWRITE = false;

    static void read(const char* path, string& body);
    static void write(const char* path, const string& body, bool append = false);
    static bool remove(const char* path);
    static void makePath(const char* path);

private:
    static void exists(const char* path) {
        //if () {
        //    throw HTTP_Exception(HTTP_Status::NOT_FOUND);
        //}
    }

    static void fail(fstream& file) {
        if (file.fail()) {
            throw HTTP_Exception(HTTP_Status::INTERNAL_SERVER_ERROR);
        }
    }
};