#include "FileManager.h"


// Reads the content of the file located at 'path' into 'body'.
void FileManager::read(const string &path, string& body) {
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
void FileManager::write(const string &path, const string& body, bool append) {
    makePath(path);
    fstream file(path, ios::out | (append ? ios::app: ios::trunc));
    //fail(file);

    // Write the body content to the file
    file.write(body.c_str(), body.size());
    file.close();
}

// Removes the file located at 'path'.
void FileManager::remove(const string &path) {
    char separator = pathSeparator(path);
    exists (path);
    fs::remove_all(path);
    size_t pos = path.find('.');
    if (pos != string::npos) {
        string dirPath = path.substr(0, path.find_last_of(separator));
        if (fs::is_empty(dirPath))
            fs::remove_all(dirPath);
    }
}

// creates all directories in a given path
void FileManager::makePath(const string& path) {
    char separator = pathSeparator(path);
    size_t pos= path.find_last_of(separator);
    string dirPath = path.substr(0, pos);
    if(!(fs::exists(dirPath))){
        if (!fs::create_directory(dirPath))
            throw HTTP_Exception(HTTP_Status::INTERNAL_SERVER_ERROR);
    }
}

// formated list of the entire file system
string FileManager::listFiles() {
    string records;
    bool flag_first = true;
    auto paths = readDirectories();
    for (const auto& path : paths) {
        size_t tabCount = count(path.begin(), path.end(), PATH_SEPARATOR);
        string file = path.substr(path.find_last_of(PATH_SEPARATOR) + 1);
        size_t pos = file.find('.');
        if (pos == string::npos) {
            if (!flag_first) {
                records.pop_back();
                records += '\n';
            }
            records += file + ':';
        }
        else
            records += ' ' + file.substr(0, pos) + ',';
        flag_first = false;
    }
    records.pop_back();
    return records;
}

// reads all directories in file system
vector<string> FileManager::readDirectories() {
    vector<string> files;
    string root_path = FILE_DIRECTORY;
    root_path += "pages";
    for (auto const& dir_entry : fs::recursive_directory_iterator(root_path)) {
        string path = dir_entry.path().string();
        if (path[path.find_last_of(PATH_SEPARATOR) + 1] != '.')
            files.push_back(path);
    }
    sort(files.begin(), files.end());
    return files;
}

// checks if a file exists
void FileManager::exists(const string &path) {
    char separator = pathSeparator(path);
    if (!fs::exists(path)) {
        string file;
        size_t path_index = path.find("pages") + 6;
        size_t pos = path.find('.') ;
        if (pos == string::npos)
            file = path.substr(path_index);
        else
            file = path.substr(path_index);
        file = file.substr(0, file.find_last_of(separator));

        string errorMsg = "Error: No records of \"" + file + '"';
        if (pos != string::npos && fs::exists(path.substr(0, path.find_last_of(separator)))) {
            string lang = path.substr(path.find_last_of(separator) + 1, 2);
            transform(lang.begin(), lang.end(), lang.begin(), ::toupper);
            errorMsg.append(" in " + lang);
        }
        throw HTTP_Exception(HTTP_Status::NOT_FOUND, errorMsg);
    }
}



