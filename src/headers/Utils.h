//
// Created by faces on 30/01/2021.
//

#ifndef SLEEPY_FACES_UTILS_H
#define SLEEPY_FACES_UTILS_H

#include <string>

struct Utils {
    static std::string exec_command(const std::string& command);
    static std::string read_last_line(const std::string& path_to_file);
    static const int MAX_DISCORD_CHARS = 2000;
};

#endif //SLEEPY_FACES_UTILS_H

