#include "fuzzer.h"
#include <thread> 
#include <vector>

Fuzzer::Fuzzer(std::string &binary_path_, int afl_count_, std::string &binary_id_, std::string &seed_path_, int time_out_, std::string work_dir_, std::string memory_limit_)
{
    memory_limit = memory_limit_;
    work_dir = work_dir_;
    seed_path = seed_path_;
    binary_path = binary_path_;
    afl_count = afl_count_;
    binary_id = binary_id_;
    time_out = time_out_;

    if(!FileIO::isDirectoryExists(work_dir_))
    {
        FileIO::createDirectory(work_dir_);
    }
    if(seed_path_.size() != 0)
    {
        if(!FileIO::isDirectoryExists(seed_path_))
        {
            std::cout << "Given path for seed path does not exists!\n";
            exit(1);
        }
        else
        {
            int file_count{};
            for(const auto &file: std::filesystem::directory_iterator(seed_path_))
            {
                if(FileIO::fileExists(file.path()))
                {
                    file_count++;
                    std::filesystem::create_directories(work_dir_+"input/");

                    std::stringstream ss_path;
                    ss_path << work_dir_ << "input/seed" << file_count;
                    
                    std::filesystem::copy(file.path(), ss_path.str(), std::filesystem::copy_options::overwrite_existing);
                    
                }
            }
            if(file_count == 0)
            {
                FileIO::createFileWithContent(work_dir_+"input/", "seed-1", "fuzz");
            }
        }
    }
    else
    {   FileIO::createFileWithContent(work_dir_+"input/", "seed-1", "fuzz");
    }
    

}

void exec(const char* cmd, std::string &filename) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        //std::cout << buffer.data();
        FileIO::writeToFile(buffer.data(), filename);
    }
}

void threadForFuzzing(std::string name, std::string command)
{
    exec(command.c_str(), name);
}

//TODO: Fix - Complete this function
std::vector<std::string> Fuzzer::foundCrash()
{
    std::vector<std::string> crashes;
    std::cout <<"this->workdir: " << this->work_dir <<"\n";
    for (const auto & entry : std::filesystem::directory_iterator(this->work_dir))
    {
        auto it = find (this->files.begin(), this->files.end(), "file_name");
        if (it == this->files.end())
        {
            crashes.push_back("file_name");
        }   
        std::cout << "path: " << entry.path() << std::endl;
    }
    
   std::cout << "foundcrash\n";
   return crashes;
}

void Fuzzer::start()
{
    std::cout << "starting fuzzer\n";
    std::string command = "afl-fuzz ";
    if(!this->seed_path.empty())
        command += "i "+ this->seed_path+" ";
    
    std::vector<std::thread> workers;

    for(int i = 1; i <= this->afl_count; i++)
    {
        std::string type = i == 1 ? "-M" : "-S";
        std::string name = "Fuzzer-"+std::to_string(i);
        std::stringstream ss_command;
        ss_command << "afl-fuzz -i " << this->work_dir << "input" << " -o " << this->work_dir << " -m " << this->memory_limit << " " << type << " " << name << " " << this->binary_path << " @@";

        workers.push_back(std::thread(threadForFuzzing, name, ss_command.str()));
    }
    
    for(auto &thread: workers){
        //thread.detach();
        thread.join();
    }

    int crash_count{};
    for(;;)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        for(auto &crash: Fuzzer::foundCrash())
        {
                crash_count++;
                std::cout << crash_count <<" th Unique Crash Found! Crash_file path: " << crash << "\n";
                this->files.push_back(crash);
        }
    }
}
