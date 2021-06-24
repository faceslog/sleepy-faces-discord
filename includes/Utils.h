//
// Created by faces on 30/01/2021.
//
#ifndef SLEEPY_FACES_UTILS_H
#define SLEEPY_FACES_UTILS_H

#include <string>
#include <stdexcept>
#include <cstdio>
#include <array>

#define MAX_DISCORD_CHARS 2000

namespace Utils
{
    std::string exec_command(const std::string& command);
};

#endif //SLEEPY_FACES_UTILS_H

