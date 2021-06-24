//
// Created by faces on 18/11/2020.
//

#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <vector>

// Sleepy Discord Lib
#include "sleepy_discord/websocketpp_websocket.h"
#include "sleepy_discord/uwebsockets_websocket.h"

#include "Logger.h"

class Client: public SleepyDiscord::DiscordClient {

public:
    Client(const std::string& token, const std::string& prefix, unsigned nbOfThreads);
    void onMessage(SleepyDiscord::Message message) override;
    void setIDWatcher(const std::string& channelID);
    void addVerifiedUser(const std::string& userID);
    void log(const std::string& content);
    std::string getPrefix();
    void setPrefix(std::string& prefix);
    std::string getIDWatcher();
    bool isUserWhitelisted(const std::string& userID);
    ~Client() = default;

private:
    std::string PREFIX;
    std::string ID_WATCHER;
    std::vector<std::string> VERIFIED_USERS;
    Logger logger = Logger("bot.log");
};

#endif //CLIENT_H
