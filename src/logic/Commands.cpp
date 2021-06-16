//
// Created by faces on 22/11/2020.
//
#include <vector>
#include <string>
#include "../headers/Commands.h"

// TO DO: REFACTOR THIS UGLY FILE WATCHER HERE
FileWatcher* watcher{nullptr};

void Commands::parse_command(Client *bot, SleepyDiscord::Message& message)
{

    std::vector<std::string> args;
    std::istringstream ss(message.content);
    std::string word;

    while (ss >> word)
    {
        args.push_back(word);
    }

    if(args.at(0) == bot->getPrefix() + "watch" && bot->isUserWhitelisted(message.author.ID))
    {
        watch(bot, message, args);
    }
    else if(args.at(0) == bot->getPrefix() + "watch-here" && bot->isUserWhitelisted(message.author.ID))
    {
        set_watching_channel(bot, message);
    }
    else if(args.at(0) == bot->getPrefix() + "get-file" && bot->isUserWhitelisted(message.author.ID))
    {
        //get_file(bot, message, args);
        bot->sendMessage(message.channelID, "Command currently disabled for security reasons !");
    }
    else if(args.at(0) == bot->getPrefix() + "prefix" && bot->isUserWhitelisted(message.author.ID))
    {
        update_prefix(bot, message, args);
    }
    else if(args.at(0) == bot->getPrefix() + "status" && bot->isUserWhitelisted(message.author.ID))
    {
        set_custom_status(bot, message, args);
    }
    else if(args.at(0) == bot->getPrefix() + "quick-scan" && bot->isUserWhitelisted(message.author.ID))
    {
        nmap_scan(bot, message, args);
    }
    else if(args.at(0) == bot->getPrefix() + "is-down")
    {
        is_website_alive(bot, message, args);
    }
    else if(args.at(0) == bot->getPrefix() + "kill" && bot->isUserWhitelisted(message.author.ID))
    {
        kill_bot(bot);
    }
    else
    {
        bot->sendMessage(message.channelID, "You don't have the permission to use this cmd or this cmd does not exist !");
        bot->addReaction(message.channelID, message.ID, "%F0%9F%98%95");
    }

}

void Commands::set_custom_status(Client *bot, SleepyDiscord::Message& message, std::vector<std::string>& args)
{
    if(args.size() == 2)
    {
        if(args.at(1).length() <= 0 || args.at(1).length() >= 20)
        {
            bot->sendMessage(message.channelID, "Cannot Set an empty string or a string length bigger than 20 as status !");
        }
        else
        {
            bot->updateStatus(args.at(1), 0, SleepyDiscord::Status::online, false);
            bot->addReaction(message.channelID, message.ID, "%E2%9C%85");
        }
    }
    else
    {
        bot->sendMessage(message.channelID, bot->getPrefix() + "status [message]");
    }
}

void Commands::watch(Client *bot, SleepyDiscord::Message& message, std::vector<std::string>& args)
{

    if(args.size() == 2)
    {
        // Blacklisted some path to prevent permission denied error
        if(args.at(1) != "/" && args.at(1) != "/etc/")
        {
            // If we had a watcher we delete it before allowing a new one else this has no effect
            delete watcher;

            // If no channel is define to output a trigger for the watcher pick the one where the command is used
            if(bot->getIDWatcher().empty())
            {
                set_watching_channel(bot, message);
                bot->sendMessage(message.channelID, "This channel has been define to output any change in the given directory you can change it by using the command `watch-here`");
            }

            // Create the fileWatcher instance that will check the current file
            watcher = new FileWatcher{args.at(1), std::chrono::milliseconds (5000)};
            bot->sendMessage(message.channelID, ("Watching the dir path: " + args.at(1)));
            watcher->processWatcher(bot);
        }
        else
        {
            bot->sendMessage(message.channelID, "This Path is currently Blacklisted ! Sorry");
        }
    }
    else
    {
        bot->sendMessage(message.channelID, bot->getPrefix() + "watch [dir/directory path]");
    }

}

void Commands::nmap_scan(Client *bot, SleepyDiscord::Message &message, std::vector<std::string> &args)
{

    if(args.size() == 2)
    {
        std::string flags = "nmap -F " + args.at(1);
        std::string result = Utils::exec_command(flags);

        // Log the nmap scan
        bot->log("Nmap scan on target: " + args.at(1) + " by user: " + message.author.username);

        if(result.size() < Utils::MAX_DISCORD_CHARS && !result.empty())
        {
            bot->sendMessage(message.channelID, "```" + result + "```");
        }
        else
        {
            bot->sendMessage(message.channelID, "``` Last line is either to big for discord or empty ! ```");
        }

    }
    else
    {
        bot->sendMessage(message.channelID, "Please provide the ipv4 to scan");
    }
}

void Commands::set_watching_channel(Client *bot, SleepyDiscord::Message &message)
{
    bot->setIDWatcher(message.channelID);
    bot->addReaction(message.channelID, message.ID, "%E2%9C%85");
}

void Commands::update_prefix(Client *bot, SleepyDiscord::Message& message, std::vector<std::string>& args)
{
    if(args.size() == 2)
    {
        bot->setPrefix(args.at(1));
        bot->sendMessage(message.channelID, "The new Prefix to ask me something is: " + args.at(1));
    }
    else
    {
        bot->sendMessage(message.channelID, "Please provide the new prefix like that: `" + bot->getPrefix() + "prefix [new_prefix]`");
    }
}

void Commands::get_file(Client *bot, SleepyDiscord::Message &message, std::vector<std::string>& args)
{
    if(args.size() == 2)
    {
        bot->log("The file: " + args.at(1) + "was requested by: " + message.author.username);
        bot->sendMessage(message.channelID, "Looking for the file...");
        bot->uploadFile(message.channelID, args.at(1),"Here goes your file: " + message.author.username);
    }
    else
    {
        bot->sendMessage(message.channelID, "Oops the command is meant to be used like that: " + bot->getPrefix() + "get-file + [full_path_to_file]");
    }
}

// Disconnect the bot
void Commands::kill_bot(Client *bot)
{
    bot->quit();
}

void Commands::is_website_alive(Client *bot, SleepyDiscord::Message &message, std::vector<std::string> &args)
{

    if(args.size() == 2)
    {
        std::string command = "curl -Is " + args.at(1) + " | head -n 1";
        std::string result = Utils::exec_command(command);
        bot->sendMessage(message.channelID, "```" + result + "```");
    }
    else
    {
        bot->sendMessage(message.channelID, "Oops the command is meant to be used like that: " + bot->getPrefix() + "is-down + [website address]");
    }
}


