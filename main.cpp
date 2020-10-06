#include <filesystem>
#include <iostream>
#include "parameters.h"
#include "fuzzer.h"
#include "file_io.h"
#include <unistd.h>


std::vector<std::string> split(const std::string& str, const std::string& delim)
{
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == std::string::npos) pos = str.length();
        std::string token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());
    return tokens;
}

bool is_number(const std::string &s) {
  return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

int main(int argc, char **argv){
    if(getuid())
    {
        std::cout << "This project must be run as root !\n";
        exit(1);
    }

    InputParser input(argc, argv);

    int fuzzer_count{};
    int time_out{};
    std::string binary_path{};
    std::string seed_path{};
    std::string work_dir{};
    std::string memory_limit{};

    if(input.cmdOptionExists("-c"))
    {
        std::string fuzzer_count_str = input.getCmdOption("-c");
        if(is_number(fuzzer_count_str))
            fuzzer_count = stoi(fuzzer_count_str);
        else
        {
            std::cout << "Please enter valid input as fuzzer count !\n";
            exit(1);
        }
        
    }
    if(input.cmdOptionExists("-b"))
    {
        
        binary_path = input.getCmdOption("-b");
        if(!FileIO::fileExists(binary_path))
        {
            std::cout << "Please enter valid binary path !\n";
            exit(1);
        }
    }
    else
    {
        std::cout << "Please enter binary path!\n";
        exit(1);
    }
    if(input.cmdOptionExists("-t"))
    {
        std::string time_out_str = input.getCmdOption("-t");
        if(is_number(time_out_str))
            time_out = stoi(time_out_str);
        else
        {
            std::cout << "Please enter valid timeout (int)!\n";
            exit(1);
        }
    }
    if(input.cmdOptionExists("-i"))
    {
        seed_path = input.getCmdOption("-i");
        if(!std::filesystem::is_directory(seed_path) || std::filesystem::is_empty(seed_path))
        {
            std::cout << "Please enter valid input address!\n";
            exit(1);
        }
    }
    if(input.cmdOptionExists("-m"))
    {
        memory_limit = input.getCmdOption("-m");
    }
    
    if(input.cmdOptionExists("-w"))
    {
        work_dir = input.getCmdOption("-w");
    }
    std::cout << "creating fuzzer\n";
    std::vector<std::string> splitted = split(binary_path,"/");
    Fuzzer fuzzer(binary_path, fuzzer_count, splitted.back(), seed_path, time_out, work_dir.size() > 0 ? work_dir : "/dev/shm/fuzzing/", memory_limit.size() > 0 ? memory_limit: "8G");

    fuzzer.start();

    return 0;
}