//
// Created by faces on 22/11/2020.
//

#include <experimental/filesystem>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <string>
#include <iostream>
#include <functional>

#include "../headers/FileWatcher.h"
#include "../headers/Utils.h"

FileWatcher::FileWatcher(const std::string& path_to_watch, std::chrono::duration<int, std::milli> delay) : path_to_watch{path_to_watch}, delay{delay} {
    for(auto &file : std::experimental::filesystem::recursive_directory_iterator(path_to_watch)) {
        paths_[file.path().string()] = std::experimental::filesystem::last_write_time(file);
    }
}

bool FileWatcher::contains(const std::string &key) {
    auto el = paths_.find(key);
    return el != paths_.end();
}

void FileWatcher::start(const std::function<void(std::string, FileStatus)> &action) {

    while (running){
        // Wait for the "delay" in ms
        std::this_thread::sleep_for(delay);

        auto it = paths_.begin();
        while (it != paths_.end()){
            if(!std::experimental::filesystem::exists(it->first)){
                action(it->first, FileStatus::erased);
                it = paths_.erase(it);
            } else {
                it++;
            }
        }

        // Check if a file was created or modified
        for(auto &file : std::experimental::filesystem::recursive_directory_iterator(path_to_watch)){
            auto current_file_last_write_time = std::experimental::filesystem::last_write_time(file);

            // File Creation
            if(!contains(file.path().string())) {
                paths_[file.path().string()] = current_file_last_write_time;
                action(file.path().string(), FileStatus::created);
                // File modification
            } else {
                if(paths_[file.path().string()] != current_file_last_write_time) {
                    paths_[file.path().string()] = current_file_last_write_time;
                    action(file.path().string(), FileStatus::modified);
                }
            }
        }
    }
}

void FileWatcher::processWatcher(Client *bot) {

    this->start([bot] (const std::string& pathToWatch, FileStatus status) -> void {

        if(!std::experimental::filesystem::is_regular_file(std::experimental::filesystem::path(pathToWatch)) && status != FileStatus::erased) {
            return;
        }

        std::string channelID = bot->getIDWatcher();

        if(!channelID.empty()){
            // TO DO: CLEAN UP THE STATUS MODIFIED BY USING A FUNCTION
            switch(status) {
                case FileStatus::created:
                    bot->sendMessage(bot->getIDWatcher(), ("``` File Created: " + pathToWatch + "```"));
                    bot->log("File Created: " + pathToWatch);
                    break;
                case FileStatus::modified: {
                    displayModifiedFile(bot, pathToWatch);
                    break;
                }
                case FileStatus::erased:
                    bot->sendMessage(bot->getIDWatcher(), ("``` File Erased: " + pathToWatch + "```"));
                    bot->log("File Erased: " + pathToWatch);
                    break;
                default: std::cout << "Error! Unknown file status.\n";
            }
        }

    });
}

void FileWatcher::displayModifiedFile(Client *bot,const std::string& path_to_modified_file) {

    bot->sendMessage(bot->getIDWatcher(), ("``` File Modified: " + path_to_modified_file + "```"));
    bot->log("File Modified: " + path_to_modified_file);

    // Get the last modified line
    std::string last_line = Utils::read_last_line(path_to_modified_file);

    // We display the last line of the file
    if(last_line.size() < Utils::MAX_DISCORD_CHARS && !last_line.empty()){
        bot->sendMessage(bot->getIDWatcher(), "``` Last line: " + last_line + "```");
    } else {
        bot->sendMessage(bot->getIDWatcher(), "``` Last line is either to big for the buffer or empty ! ```");
    }

}

FileWatcher::~FileWatcher() = default;

