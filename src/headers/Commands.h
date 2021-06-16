//
// Created by faces on 22/11/2020.
//

#ifndef SLEEPY_FACES_COMMANDS_H
#define SLEEPY_FACES_COMMANDS_H

#include <iostream>
#include "FileWatcher.h"
#include "Utils.h"

struct Commands {
    static void parse_command(Client *bot, SleepyDiscord::Message& message);
    static void set_custom_status(Client *bot, SleepyDiscord::Message& message, std::vector<std::string>& args);
    static void set_watching_channel(Client *bot, SleepyDiscord::Message& message);
    static void update_prefix(Client *bot, SleepyDiscord::Message& message, std::vector<std::string>& args);
    static void watch(Client *bot, SleepyDiscord::Message& message, std::vector<std::string>& args);
    static void nmap_scan(Client *bot, SleepyDiscord::Message& message, std::vector<std::string>& args);
    static void get_file(Client *bot, SleepyDiscord::Message& message, std::vector<std::string>& args);
    static void is_website_alive(Client *bot, SleepyDiscord::Message& message, std::vector<std::string>& args);
    // Command to stop the bot script
    static void kill_bot(Client *bot);
};


#endif //SLEEPY_FACES_COMMANDS_H
