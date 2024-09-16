#pragma once
#include "Config.h"
#include "HTTP_Status.h"

class FileManager {
public:
    static constexpr bool APPEND = true;
    static constexpr bool OVERWRITE = false;

    // reads file content
    static void read(const string &path, string& body);

    // writes into a file, either appends or overwrites
    // creating needed and files directories in the process
    static void write(const string &path, const string& body, bool append = false);

    // deletes a file
    static void remove(const string &path);

    // creates all directories in a given path
    static void makePath(const string& path);

    // formated list of the entire file system
    static string listFiles();

    // reads all directories in file system
    static vector<string> readDirectories();

    // checks if a file exists
    static void exists(const string& path);

    static constexpr auto FILE_DIRECTORY = "C:/temp/";
    static constexpr auto PATH_SEPARATOR = '\\';
    static constexpr auto POSIX_PATH_SEPARATOR = '/';

private:
    // checks which path format is used (windows/posix)
    static char pathSeparator(const string& path) {
        if (path.find(PATH_SEPARATOR) != string::npos)
            return PATH_SEPARATOR;
        else
            return POSIX_PATH_SEPARATOR;
    }

    // checks and throw exception file operation as failed
    static void fail(fstream& file) {
        if (file.fail()) {
            throw HTTP_Exception(HTTP_Status::INTERNAL_SERVER_ERROR);
        }
    }
};