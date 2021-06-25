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

// Sleepy Discord Lib
#include "sleepy_discord/websocketpp_websocket.h"
#include "sleepy_discord/uwebsockets_websocket.h"

#include "Logger.h"
#include "Utils.h"

class Client: public SleepyDiscord::DiscordClient {

public:
    Client(const std::string& token, const std::string& prefix, unsigned nbOfThreads);
    ~Client() = default;

    void setPrefix(std::string& _prefix);
    void addVerifiedUser(const std::string& userID);
    void log(const std::string& content);

    std::string getPrefix();

private:
    std::string prefix;
    std::vector<std::string> verifiedUsers;
    Logger logger;

    void onMessage(SleepyDiscord::Message message) override;
    bool isUserWhitelisted(const std::string& userID);

    void parseCommand(SleepyDiscord::Message& message);
    void updatePrefix(SleepyDiscord::Message& message, std::vector<std::string>& args);
    // void watch(SleepyDiscord::Message& message, std::vector<std::string>& args);
    void nmapScan(SleepyDiscord::Message& message, std::vector<std::string>& args);
    void isWebsiteAlive(SleepyDiscord::Message& message, std::vector<std::string>& args);
    void killBot();
};

#endif //CLIENT_H
