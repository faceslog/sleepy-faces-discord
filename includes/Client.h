//
// Created by faces on 18/11/2020.
//
#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <vector>
#include <iostream>
#include <vector>
#include <string>

// Sleepy Discord
#include "sleepy_discord/sleepy_discord.h"
#include "Logger.h"
#include "Utils.h"

class Client: public SleepyDiscord::DiscordClient {

public:
    explicit Client(std::string token, std::string prefix, uint16_t nbOfThreads);
    void addVerifiedUser(std::string userID);
    void log(const std::string& content);

private:
    void onMessage(SleepyDiscord::Message message) override;
    bool isUserWhitelisted(const std::string& userID);
    void setPrefix(std::string _prefix);
    std::string getPrefix();
    void parseCommand(const SleepyDiscord::Message& message);
    void help(const SleepyDiscord::Message& message);
    void updatePrefix(const SleepyDiscord::Message& message, std::vector<std::string>& args);
    void ark(const SleepyDiscord::Message& message, std::vector<std::string>& args);
    void killBot(const SleepyDiscord::Message& message);

    std::string prefix;
    std::vector<std::string> verifiedUsers;
    Logger logger;
};

#endif //CLIENT_H
