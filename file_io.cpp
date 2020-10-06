#include "file_io.h"

void FileIO::createFileWithContent(std::string path, std::string fileName, std::string content)
{
    std::filesystem::create_directories(path);
    std::ofstream o(path+"/"+fileName);
    o << content << "\n";
    o.close();
}

bool FileIO::fileExists (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0) && (buffer.st_mode & S_IFREG); 
}

bool FileIO::isDirectoryExists (const std::string& path)
{
    return std::filesystem::is_directory(path);
}

void FileIO::writeToFile(std::string filetext, std::string &file_name) {
    std::ofstream myfile;
    myfile.open (file_name, std::fstream::app);
    myfile <<filetext;
    myfile.close();
}
void FileIO::createDirectory(const std::string &path)
{
    std::filesystem::create_directory(path);
 
 }