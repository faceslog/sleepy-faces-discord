//
// Created by faces on 18/11/2020.
//
#include "sleepy_discord/websocketpp_websocket.h"

#include "../headers/Client.h"
#include "../headers/Commands.h"

void Client::onMessage(SleepyDiscord::Message message) {

    if(!message.author.bot)
    {
        if(message.serverID.string().empty())
        {
            sendMessage(message.channelID, "Oops I cannot answer to DMs");
            return;
        }

        if(message.startsWith(this->PREFIX))
        {
            log('[' + message.author.username + "] command: " + message.content);
            Commands::parse_command(this, message);
        }
    }

}

void Client::setIDWatcher(const std::string &channelID) {
    this->ID_WATCHER = channelID;
}

void Client::addVerifiedUser(const std::string& userID) {
    this->VERIFIED_USERS.push_back(userID);
}

void Client::log(const std::string &content) {
    logger.WriteLine(content);
}

std::string Client::getPrefix() {
    return this->PREFIX;
}

void Client::setPrefix(std::string &prefix) {
    PREFIX = prefix;
}

std::string Client::getIDWatcher() {
    return this ->ID_WATCHER;
}

bool Client::isUserWhitelisted(const std::string& user_id) {
    // Check whether an element exists in the vector or not
    return std::find(VERIFIED_USERS.begin(), VERIFIED_USERS.end(), user_id) != VERIFIED_USERS.end();
}


