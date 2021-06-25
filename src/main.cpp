#include "Client.h"

int main()
{
    auto* config = new Utils::Config();
    // Setup the bot
    Utils::LoadConfig(config);

    if(config->token.empty() || config->prefix.empty() || config->users.empty())
    {
        delete config;
        std::cout << "The Config File is incorrect !" << std::endl;
        return EXIT_FAILURE;
    }

    Client bot(config->token, config->prefix, 2);
    // Add the whitelisted user to the bot
    for(const auto& user: config->users)
    {
        bot.addVerifiedUser(user);
        std::cout << "New verified user ID: " << user << std::endl;
    }

    delete config;
    bot.run();

    return EXIT_SUCCESS;
}

