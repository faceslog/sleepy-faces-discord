//
// Created by faces on 30/01/2021.
//
#ifndef SLEEPY_FACES_UTILS_H
#define SLEEPY_FACES_UTILS_H

#include <unistd.h>
#include <string>
#include <stdexcept>
#include <array>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

#define MAX_DISCORD_CHARS 2000
#define CONFIG_PATH "/bot.config"

namespace Utils
{
    struct Config {
        std::string token;
        std::string prefix;
        std::vector<std::string> users;
    };

    void LoadConfig(Config* config);
    std::string GetCurrentWorkingDir();
    std::string ExecCommand(const std::string& command);
};

#endif //SLEEPY_FACES_UTILS_H

