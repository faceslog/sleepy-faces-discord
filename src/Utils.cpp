//
// Created by faces on 30/01/2021.
//
#include "../includes/Utils.h"

std::string Utils::exec_command(const std::string &command) {

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
