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
    return experimental::filesystem::remove_all(path);
}

void FileManager::makePath(const char* path) {
    experimental::filesystem::path dir(path);
    if(!(experimental::filesystem::exists(dir))){
        if (!experimental::filesystem::create_directory(dir))
            throw HTTP_Exception(HTTP_Status::INTERNAL_SERVER_ERROR);
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


void FileManager::read_directory(const std::string& path, vector<string>& v)
{
    experimental::filesystem::path dirPath = path;
    if (experimental::filesystem::exists(dirPath) && experimental::filesystem::is_directory(dirPath)) {
        for (const auto& entry : experimental::filesystem::directory_iterator(dirPath)) {
            v.push_back(entry.path().string());
        }
    } else {
        throw HTTP_Exception(HTTP_Status::NOT_FOUND);
    }
}