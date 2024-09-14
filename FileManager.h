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
    static string list_records();
    static string list_files();
    static void makePath(const string& path);
    static vector<string> readDirectories();
    static  void readDirectory(const string& name, vector<string>& v);

private:
    static void exists(const string &path);

    static void fail(fstream& file) {
        if (file.fail()) {
            throw HTTP_Exception(HTTP_Status::INTERNAL_SERVER_ERROR);
        }
    }
};