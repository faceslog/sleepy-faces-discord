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
    explicit Client(const std::string& token, const std::string& prefix, unsigned nbOfThreads);
    void addVerifiedUser(std::string userID);
    void log(const std::string& content);

private:
    std::string prefix;
    std::vector<std::string> verifiedUsers;
    Logger logger;

    void onMessage(SleepyDiscord::Message message) override;
    bool isUserWhitelisted(const std::string& userID);
    void setPrefix(std::string _prefix);
    std::string getPrefix();
    void parseCommand(SleepyDiscord::Message& message);
    void help(SleepyDiscord::Message& message);
    void updatePrefix(SleepyDiscord::Message& message, std::vector<std::string>& args);
    void ark(SleepyDiscord::Message& message, std::vector<std::string>& args);
    void killBot();
};

#endif //CLIENT_H
