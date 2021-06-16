cmake_minimum_required(VERSION 3.17)
project(sleepy_faces)

set(CMAKE_CXX_STANDARD 17)

set(PROJECT_SOURCES
        src/main.cpp
        src/logic/Client.cpp
        src/logic/Commands.cpp
        src/logic/FileWatcher.cpp
        src/logic/Logger.cpp
        src/logic/Utils.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

add_subdirectory(assets/sleepy-discord)
target_link_libraries(sleepy_faces sleepy-discord)
target_link_libraries(sleepy_faces stdc++fs)
