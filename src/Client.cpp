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
    else if(args.at(0) == getPrefix() + "dump" && isUserWhitelisted(message.author.ID))
    {
        dump(message, args);
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
    else if(args.at(0) == getPrefix() + "help")
    {
        help(message);
    }
    else
    {
        sendMessage(message.channelID, "You don't have the permission to use this cmd or this cmd does not exist !");
        addReaction(message.channelID, message.ID, "%F0%9F%98%95");
    }

}

void Client::help(SleepyDiscord::Message &message)
{
    std::string help_str;

    help_str = getPrefix() + "prefix --> To modify the bot's prefix\n";
    help_str += getPrefix() + "quick-scan [ip] --> To scan an IP or Domain with Nmap\n";
    help_str += getPrefix() + "is-down [website-link] --> Test if the given website is available\n";
    help_str += getPrefix() + "kill --> Stop the bot\n";

    sendMessage(message.channelID, help_str);
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

void Client::dump(SleepyDiscord::Message &message, std::vector<std::string>& args)
{
    if(not message.attachments.empty())
    {
        if(args.size() == 2)
        {
            static const std::string file_name("dump.txt");
            std::string cmd{};
            std::string result{};

            if(args.at(1) == "bytes")
            {
                cmd = "curl " + message.attachments.at(0).url + " | xxd -i";
                log('['+message.author.username + "] " + cmd);
                result = Utils::ExecCommand(cmd);
            }
            else
            {
                cmd = "curl " + message.attachments.at(0).url + " | xxd";
                log('['+message.author.username + "] " + cmd);
                result = Utils::ExecCommand(cmd);
            }

            std::ofstream out(file_name);
            out << result;
            out.close();

            uploadFile(message.channelID, file_name, "File Data:");
            std::remove(file_name.c_str());
        }
        else
        {
            sendMessage(message.channelID, "Oops the command is meant to be used like that: " + getPrefix() + "dump + [bytes | binary]");
        }
    }
    else
    {
        sendMessage(message.channelID, "Please send the file you want to dump with this command");
    }
}


