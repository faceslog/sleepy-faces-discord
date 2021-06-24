//
// Created by faces on 08/01/2021.
//

#ifndef SLEEPY_FACES_LOGGER_H
#define SLEEPY_FACES_LOGGER_H

#include <string>
#include <mutex>
#include <fstream>
#include <ctime>

/**
 * To manage the file and the lock resources, we use the RAII concept. The resources will be managed by objects.
 * The ctor will initialize the file stream. The second resource, the locking object is needed in the output function.
 *  This output function which will add a logging message can be called by different threads. The std class "lock_guard"
 *  implements the RAII concept. We also need to present to create a copy of a class instance. We allow to pass the class
 *  instance into another scope. In case we move the class instance we can steal the resources from the source class and
 *  transfer it to the target class. As the source class instance will be deleted this steal of resources is allowed. The
 *  copy ctor and copy assignment operator are disabled and the move ctor and move assignment is implemented
 * */
class Logger {

public:
    // Constructor with the file where the logs should be saved
    Logger(const std::string& fileName);

    // Disable copy ctor and copy assignment
    Logger(const Logger&) = delete;
    Logger& operator= (const Logger&) = delete;

    // Move ctor and move assignment
    Logger(Logger&& other)
    {
        mStream.close();
        mStream = move(other.mStream);
    }

    Logger& operator=(Logger&& other)
    {
        mStream.close();
        mStream = move(other.mStream);

        return *this;
    }

    // Write to log File
    void WriteLine(const std::string& content);
    //Destructor
    ~Logger();

private:
    std::ofstream mStream;
    std::mutex mMutex;
};


#endif //SLEEPY_FACES_LOGGER_H
