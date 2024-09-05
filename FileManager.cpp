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
void FileManager::append(const char *path, char *body) {
    makePath(path);
    fstream file(path, ios::out | ios::app);
    fail(file);

    // Write the body content to the file
    file.write(body, strlen(body));
    file.close();
}

// Creates a new file and path writes 'body' to it.
void FileManager::write(const char* path, char* body) {
    makePath(path);
    fstream file(path, ios::out | ios::trunc);
    fail(file);

    // Write the body content to the file
    file.write(body, strlen(body));
    file.close();
}

// Removes the file located at 'path'.
void FileManager::remove(const char* path) {
    exists(path);
    boost::filesystem::path p(path);
    boost::filesystem::remove(p);
}

void FileManager::makePath(const char* path) {
    boost::filesystem::path dir(path);
    if(!(boost::filesystem::exists(dir))){
        if (!boost::filesystem::create_directory(dir))
            throw HTTP_Status(HTTP_Status::INTERNAL_SERVER_ERROR);
    }
};