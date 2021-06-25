## SLEEPY-FACES DISCORD BOT C++20

### This repo use https://github.com/yourWaifu/sleepy-discord

#### This my first C++ project, and the code really need to be improved ! The goal of this bot was to learn about the basics of the C++ language.

*Simple discord bot that can perform shell command via discord and display the result as message currently I added only nmap command to prevent users using bash command on my server
I also implemented a simple log system that log each shell and bot command that is done by a user*

Please setup a bot.config file and follow the bot.config example

**Make sure to install these packages**

`sudo apt-get update -y`

`sudo apt-get install -y libcurl4-openssl-dev`

`sudo apt-get install libssl-dev or openssl-devel` Depends on the package manager

If you want to run quick-scan command be sure to have installed **nmap**:
`sudo apt-get install nmap`

TO DO:
- Secure :poppen() to make more commands available through the bot
- Refactor the code that is bad written and hard to understand
