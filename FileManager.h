#pragma once
#include "Config.h"
#include "HTTP_Status.h"

class FileManager {
public:
    static constexpr bool APPEND = true;
    static constexpr bool OVERWRITE = false;

    static void read(const string &path, string& body);
    static void write(const string &path, const string& body, bool append = false);
    static void remove(const string &path);
    static string listFiles();
    static void makePath(const string& path);
    static vector<string> readDirectories();
    static void exists(const string& path);

    static constexpr auto FILE_DIRECTORY = "C:/temp/";
    static constexpr auto PATH_SEPARATOR = '\\';
    static constexpr auto POSIX_PATH_SEPARATOR = '/';

private:

    static char pathSeparator(const string& path) {
        if (path.find(PATH_SEPARATOR) != string::npos)
            return PATH_SEPARATOR;
        else
            return POSIX_PATH_SEPARATOR;
    }

    static void fail(fstream& file) {
        if (file.fail()) {
            throw HTTP_Exception(HTTP_Status::INTERNAL_SERVER_ERROR);
        }
    }
};