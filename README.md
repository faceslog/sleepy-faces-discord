## SLEEPY-FACES DISCORD BOT C++ 17 (LINUX ONLY)

### This repo use https://github.com/yourWaifu/sleepy-discord

#### This my first C++ project and the code really need to be improved ! The goal of this bot was to learn about the basics of the C++ language.

**Make sure to change the path in the main.cpp before compiling in case you don't want to use the bot.config into the cmake-build-debug/ folder or at the root of the bot script**

For example change to /../bot.config instead of /bot.config to use the config file at the root of this repo during development

*Simple discord bot that can perform shell command via discord and display the result as message currently I added only nmap command to prevent users using bash command on my server
I also implented a simple log system that log each shell and bot command that is done by a user*

Please setup a bot.config file and follow the bot.config example

**Make sure to install these packages**

`sudo apt-get update -y`

`sudo apt-get install -y libcurl4-openssl-dev`

`sudo apt-get install libssl-dev or openssl-devel` Depends on the package manager

If you want to run quick-scan command be sure to have installed **nmap**:
`sudo apt-get install nmap`

TO DO:
- Securize :poppen() to make more commands available through the bot
- Refactor the code that is bad written and hard to understand
