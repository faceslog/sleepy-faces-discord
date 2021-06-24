//
// Created by faces on 08/01/2021.
//

#include "../includes/Logger.h"

// Ctor
Logger::Logger(const std::string& fileName) {
    mStream.open(fileName);

    if(mStream.fail())
    {
        throw std::iostream::failure("Cannot open file:" + fileName);
    }
}

void Logger::WriteLine(const std::string& content) {

    std::lock_guard lock(mMutex);

    // Format the tm time using strftime function
    time_t raw_time;
    struct tm * time_info;
    char buffer[80];

    time (&raw_time);
    time_info = localtime(&raw_time);

    strftime(buffer,sizeof(buffer),"%d-%m-%Y %H:%M:%S", time_info);
    std::string date(buffer);
    mStream << '[' << date << ']' << ": " << content << std::endl;
}

// Dtor
Logger::~Logger() {
    // Close the opened resource
    mStream.close();
}


