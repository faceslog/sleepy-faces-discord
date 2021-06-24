//
// Created by faces on 22/11/2020.
//

#ifndef SLEEPY_FACES_FILEWATCHER_H
#define SLEEPY_FACES_FILEWATCHER_H

#include <experimental/filesystem>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <string>
#include <functional>
#include <iostream>

#include "Client.h"
#include "Utils.h"

enum class FileStatus {created, modified, erased};

class FileWatcher {

public:
    std::string path_to_watch;

    // Time interval at which we check the base folder for changes
    std::chrono::duration<int, std::milli> delay{};

    // Keep a record of files from the base directory and their last modification time
    FileWatcher(const std::string& path_to_watch, std::chrono::duration<int, std::milli> delay);

    // Monitor "path_to_watch" for changes and in case of a change execute the user supplied
    void start(const std::function<void (std::string, FileStatus)> &action);
    void processWatcher(Client *bot);
    static void displayModifiedFile(Client *bot, const std::string& path_to_modified_file);

    // Destructor
    ~FileWatcher();

private:
    std::unordered_map<std::string, std::experimental::filesystem::file_time_type> paths_;
    bool running = true;

    // Check if 'paths_" contains a given key
    // If your compiler supports C++20 use paths_.contains(keys) instead of this function
    bool contains(const std::string &key);
};
#endif //SLEEPY_FACES_FILEWATCHER_H
