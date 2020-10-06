#ifndef SHELLPHUZZ_CPP_HEADERS_FILE_IO
#define SHELLPHUZZ_CPP_HEADERS_FILE_IO

#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <sys/stat.h>

namespace FileIO 
{   bool fileExists (const std::string& name);
    bool isDirectoryExists(const std::string& path);
    void createFileWithContent(std::string path, std::string fileName, std::string content);
    void writeToFile(std::string fileText, std::string &fileName);
    void createDirectory(const std::string &path);
}
#endif