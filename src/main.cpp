#include "Client.h"

int main()
{
    auto* config = new Utils::Config();
    // Set up the bot
    Utils::LoadConfig(config);

    if(config->token.empty() || config->prefix.empty() || config->users.empty())
    {
        delete config;
        std::cout << "The Config File is incorrect !" << std::endl;
        return EXIT_FAILURE;
    }

    Client bot(std::move(config->token), std::move(config->prefix), 2);
    // Add the whitelisted user to the bot
    for(auto& user: config->users)
    {
        std::cout << "Adding user ID: " << user << std::endl;
        bot.addVerifiedUser(std::move(user));
    }

    delete config;
    bot.run();

    return EXIT_SUCCESS;
}

