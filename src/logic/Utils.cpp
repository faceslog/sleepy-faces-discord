//
// Created by faces on 30/01/2021.
//

#include "../headers/Utils.h"
#include <stdexcept>
#include <cstdio>
#include <array>
#include <fstream>
#include <cmath>

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

std::string Utils::read_last_line(const std::string &path_to_file) {
    std::string last_line;

#define BUFF_SIZE 4096

    last_line.clear();
    char buff[BUFF_SIZE];

    std::ifstream file;
    file.open(path_to_file);

    if(file.is_open())
    {
        file.seekg(0, std::ios_base::end); // Go to end of the file
        int length = file.tellg(); // Find out how large it is
        file.seekg(length-fmin(length, BUFF_SIZE),std::ios::beg); // Seek back from end a shot ways

        // Read in each line of the file until we're done
        buff[0] = 0;

        do{
            // Skip empty lines or lines that start with whitespace
            if(!isspace(buff[0]) && buff[0] != 0){
                last_line = buff;
            }

        } while (file.getline(buff, BUFF_SIZE));

        file.close();
    }

    return last_line;
}
