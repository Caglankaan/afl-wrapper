#include <iostream>
#include <vector>
#include <algorithm>
#include <vector>
#include "file_io.h"
class Fuzzer{
    public:
        std::string binary_path;
        std::string binary_id;
        std::string seed_path;
        std::string work_dir;
        std::string memory_limit;
        int afl_count;
        int time_out;
        std::vector<std::string> files;
        Fuzzer (std::string &binary_path_, int afl_count_, std::string &binary_id_, std::string &seed_path_, int time_out_, std::string work_dir, std::string memory_limit);
        void start();
        std::vector<std::string> foundCrash();

};