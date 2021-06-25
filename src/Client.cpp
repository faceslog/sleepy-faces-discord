//
// Created by faces on 18/11/2020.
//
#include "Client.h"

Client::Client(const std::string& token, const std::string& prefix, unsigned int nbOfThreads) :SleepyDiscord::DiscordClient(token, static_cast<char>(nbOfThreads)), logger("bot.log")
{
    this->prefix = prefix;
    log("Bot Starting !");
}

void Client::onMessage(SleepyDiscord::Message message)
{
    if(!message.author.bot)
    {
        if(message.startsWith(prefix))
        {
            // If it's a DM the serverID.string will be empty
            if(message.serverID.string().empty())
            {
                sendMessage(message.channelID, "Oops I cannot answer to DMs");
            }
            else
            {
                log('[' + message.author.username + "] command: " + message.content);
                parseCommand(message);
            }
        }
    }
}

void Client::addVerifiedUser(const std::string& userID)
{
    verifiedUsers.push_back(userID);
}

void Client::log(const std::string &content)
{
    logger.WriteLine(content);
}

std::string Client::getPrefix()
{
    return prefix;
}

void Client::setPrefix(std::string &_prefix)
{
    prefix = _prefix;
}

bool Client::isUserWhitelisted(const std::string& user_id)
{
    // Check whether an element exists in the vector or not
    return std::find(verifiedUsers.begin(), verifiedUsers.end(), user_id) != verifiedUsers.end();
}

void Client::parseCommand(SleepyDiscord::Message& message)
{
    std::vector<std::string> args;
    std::istringstream ss(message.content);
    std::string word;

    while (ss >> word)
    {
        args.push_back(word);
    }

    // TO DO: REFACTOR THIS UGLY SHIT !!!!!
    if(args.at(0) == getPrefix() + "prefix" && isUserWhitelisted(message.author.ID))
    {
        updatePrefix(message, args);
    }
    else if(args.at(0) == getPrefix() + "quick-scan" && isUserWhitelisted(message.author.ID))
    {
        nmapScan(message, args);
    }
    else if(args.at(0) == getPrefix() + "is-down")
    {
        isWebsiteAlive(message, args);
    }
    else if(args.at(0) == getPrefix() + "kill" && isUserWhitelisted(message.author.ID))
    {
        killBot();
    }
    else
    {
        sendMessage(message.channelID, "You don't have the permission to use this cmd or this cmd does not exist !");
        addReaction(message.channelID, message.ID, "%F0%9F%98%95");
    }

}

void Client::nmapScan(SleepyDiscord::Message &message, std::vector<std::string>& args)
{
    if(args.size() == 2)
    {
        std::string flags = "nmap -F " + args.at(1);
        std::string result = Utils::ExecCommand(flags);

        // Log the nmap scan
        log("Nmap scan on target: " + args.at(1) + " by user: " + message.author.username);

        if(result.size() < MAX_DISCORD_CHARS && !result.empty())
        {
            sendMessage(message.channelID, "```" + result + "```");
        }
        else
        {
           sendMessage(message.channelID, "``` Last line is either to long for discord or empty ! ```");
        }
    }
    else
    {
        sendMessage(message.channelID, "Please provide the ip or domain to scan");
    }
}

void Client::updatePrefix(SleepyDiscord::Message& message, std::vector<std::string>& args)
{
    if(args.size() == 2)
    {
        setPrefix(args.at(1));
        sendMessage(message.channelID, "The new Prefix to ask me something is: " + args.at(1));
    }
    else
    {
        sendMessage(message.channelID, "Please provide the new prefix like that: `" + getPrefix() + "prefix [new_prefix]`");
    }
}

void Client::isWebsiteAlive(SleepyDiscord::Message &message, std::vector<std::string>& args)
{
    if(args.size() == 2)
    {
        std::string command = "curl -Is " + args.at(1) + " | head -n 1";
        std::string result = Utils::ExecCommand(command);
        sendMessage(message.channelID, "```" + result + "```");
    }
    else
    {
        sendMessage(message.channelID, "Oops the command is meant to be used like that: " + getPrefix() + "is-down + [website address]");
    }
}

void Client::killBot() {  quit(); }


