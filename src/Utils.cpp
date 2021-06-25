//
// Created by faces on 30/01/2021.
//
#include "Utils.h"

void Utils::LoadConfig(Config* config)
{
    std::string path = GetCurrentWorkingDir();
    std::cout << "[Current Path]: " << path << std::endl;
    std::ifstream confFile(path + CONFIG_PATH);

    if(confFile.is_open())
    {
        std::string line;
        while(getline(confFile, line))
        {
            line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());

            if(line[0] == '#' || line.empty()) continue;

            auto delimiterPos = line.find('=');
            auto name = line.substr(0, delimiterPos);
            auto value = line.substr(delimiterPos + 1);

            if(name == "prefix") config->prefix = value;

            if(name == "token") config->token = value;

            if(name.find("user") != std::string::npos)  config->users.push_back(value);
        }

        confFile.close();
    }
    else
    {
        std::cout << "Unable to Open Config File !" << std::endl;
    }
}

std::string Utils::GetCurrentWorkingDir()
{
    char buff[FILENAME_MAX];
    getcwd( buff, FILENAME_MAX );

    return std::string(buff);
}

std::string Utils::ExecCommand(const std::string& command) {

    int exit_status = 0;
    auto p_pipe = ::popen(command.c_str(), "r");

    if(p_pipe == nullptr) throw std::runtime_error("Cannot open the pipe");

    std::array<char, 256> buffer{};
    std::string result;

    while(not std::feof(p_pipe))
    {
        auto bytes = std::fread(buffer.data(), 1, buffer.size(), p_pipe);
        result.append(buffer.data(), bytes);
    }

    auto rc = ::pclose(p_pipe);
    if(WIFEXITED(rc))
    {
        exit_status = WEXITSTATUS(rc);
    }

    result += "\nExit status: " + std::to_string(exit_status);

    return result;
}
