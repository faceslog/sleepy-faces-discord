//
// Created by faces on 18/11/2020.
//
#include "Client.h"

Client::Client
(
    std::string token,
    std::string prefix,
    uint16_t nbOfThreads
) : SleepyDiscord::DiscordClient(std::move(token), static_cast<char>(nbOfThreads)), logger("bot.log")
{
    this->prefix = std::move(prefix);
    log("Bot Starting !");
}

void Client::onMessage(SleepyDiscord::Message message)
{
    if(message.author.bot) return;
    if(!message.startsWith(prefix)) return;
    if(message.serverID.string().empty())
    {
        sendMessage(message.channelID, "Oops I cannot answer to DMs");
        return;
    }

    if(!isUserWhitelisted(message.author.ID)) return;

    log('[' + message.author.username + "] command: " + message.content);
    parseCommand(message);
}

void Client::addVerifiedUser(std::string userID)
{
    verifiedUsers.push_back(std::move(userID));
}

void Client::log(const std::string &content)
{
    logger.WriteLine(content);
}

void Client::setPrefix(std::string _prefix)
{
    prefix = std::move(_prefix);
}

std::string Client::getPrefix()
{
    return prefix;
}

bool Client::isUserWhitelisted(const std::string& user_id)
{
    // Check whether an element exists in the vector or not
    return std::find(verifiedUsers.begin(), verifiedUsers.end(), user_id) != verifiedUsers.end();
}

void Client::parseCommand(const SleepyDiscord::Message& message)
{
    std::vector<std::string> args;
    std::istringstream ss(message.content);
    std::string word;

    while (ss >> word)
    {
        args.push_back(word);
    }

    // TO DO: REFACTOR THIS SHITTY ELSE IF !!!!!
    if(args.at(0) == getPrefix() + "prefix")
    {
        updatePrefix(message, args);
    }
    else if(args.at(0) == getPrefix() + "ark")
    {
        ark(message, args);
    }
    else if(args.at(0) == getPrefix() + "kill")
    {
        killBot(message);
    }
    else if(args.at(0) == getPrefix() + "help")
    {
        help(message);
    }
    else
    {
        sendMessage(message.channelID, "You don't have the permission to use this cmd or this cmd does not exist !");
    }
}

void Client::help(const SleepyDiscord::Message &message)
{
    std::string help_str;

    help_str = getPrefix() + "prefix --> To modify the bot' prefix\n";
    help_str += getPrefix() + "ark --> [ start | stop | update ] To run a command on the ark server\n";
    help_str += getPrefix() + "kill --> Stop the bot\n";

    sendMessage(message.channelID, help_str);
}

void Client::updatePrefix(const SleepyDiscord::Message& message, std::vector<std::string>& args)
{
    if(args.size() != 2)
    {
        sendMessage(message.channelID, "Please provide the new prefix like that: `" + getPrefix() + "prefix [new_prefix]`");
        return;
    }

    setPrefix(args.at(1));
    sendMessage(message.channelID, "The new Prefix to ask me something is: " + args.at(1));
}

void Client::ark(const SleepyDiscord::Message& message, std::vector<std::string>& args)
{
    if(args.size() != 2)
    {
        sendMessage(message.channelID, "Please provide an argument !");
        return;
    }

    if(args.at(1) != "start" && args.at(1) != "stop" && args.at(1) != "update")
    {
        sendMessage(message.channelID, "Invalid argument !");
        return;
    }

    std::string output {};
    try
    {
        sendMessage(message.channelID, "Running command please wait for output ...");
        output = Utils::ExecCommand("bash facesark " + args.at(1));
    }
    catch (std::runtime_error&)
    {
        sendMessage(message.channelID, "Failed to run cmd !");
        return;
    }

    // Verify that the output can be sent
    if(output.size() > MAX_DISCORD_CHARS || output.empty())
    {
        sendMessage(message.channelID, "Result is either too large for discord or empty !");
        return;
    }

    sendMessage(message.channelID, "```" + output + "```");
}

void Client::killBot(const SleepyDiscord::Message& message)
{
    sendMessage(message.channelID, "Stopping bot ...");
    quit();
}


