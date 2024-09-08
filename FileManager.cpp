#include "FileManager.h"


// Reads the content of the file located at 'path' into 'body'.
void FileManager::read(const char* path, string& body) {
    exists(path);
    fstream file(path, ios::in);
    fail(file);

    // Read file content into body
    file.seekg(0, ios::end);
    streamsize size = file.tellg();
    file.seekg(0, ios::beg);
    char* buffer = new char[size + 1];
    file.read(buffer, size);
    buffer[size] = '\0';
    body = buffer;

    delete[] buffer;
    file.close();
}

// Creates a new file and path writes 'body' to it.
void FileManager::write(const char* path, const string& body, bool append) {
    makePath(path);
    fstream file(path, ios::out | (append ? ios::app : ios::trunc));
    fail(file);

    // Write the body content to the file
    file.write(body.c_str(), body.size());
    file.close();
}

// Removes the file located at 'path'.
bool FileManager::remove(const char* path) {
    return false;
}

void FileManager::read_directory(const std::string& name, vector<string>& v)
{
    std::string pattern(name);
    pattern.append("/");
    WIN32_FIND_DATA data;
    HANDLE hFind;
    if ((hFind = FindFirstFile(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE) {
        do {
            v.push_back(data.cFileName);
        } while (FindNextFile(hFind, &data) != 0);
        FindClose(hFind);
    }
}

string FileManager::list_records() {
    vector<string> files;
    ostringstream str;
    read_directory("library", files);
    for (const auto &file: files) {
        vector<string> languages;
        str << file << ": " ;
        read_directory("library/" + file, languages);
        for (const auto &lang: languages) {
            str << lang << (lang == languages.back() ? ", " : "\n");
        }
    }
    return str.str();
}
