#include <iostream>
#include <string>
#include <cstdio>
#include <unistd.h>

#include "../includes/Client.h"

struct Config {
    std::string token;
    std::string prefix;
    std::vector<std::string> users;
};

std::string GetCurrentWorkingDir( ) {
    char buff[FILENAME_MAX];
    getcwd( buff, FILENAME_MAX );
    std::string current_working_dir(buff);
    return current_working_dir;
}

void LoadConfig(Config* config){

    std::string path = GetCurrentWorkingDir();
    std::cout << "[Current Path]: " << path << std::endl;
    std::ifstream confFile(path + "/bot.config");

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

           if(name == "prefix")
           {
               config->prefix = value;
           }

            if(name == "token")
            {
                config->token = value;
            }

           if(name.find("user") != std::string::npos){
               config->users.push_back(value);
           }
        }

        confFile.close();
    }
    else
    {
        std::cout << "Unable to Open Config File !" << std::endl;
    }
}

int main() {

    auto* config = new Config();

    // Setup the bot
    LoadConfig(config);

    if(!config->token.empty() && !config->prefix.empty() && !config->users.empty())
    {
        Client bot(config->token, config->prefix, 2);

        // Add the whitelisted user to the bot
        for(const auto& user: config->users){
            bot.addVerifiedUser(user);
            std::cout << "New verified user ID: " << user << std::endl;
        }

        // Free the config space since we do not need it here anymore
        delete config;
        // Connect the bot to make it available
        bot.run();
    }
    else
    {
        delete config;
        std::cout << "The Config File is incorrect !" << std::endl;
    }

    return EXIT_SUCCESS;
}

