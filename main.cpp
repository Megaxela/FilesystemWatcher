#include <iostream>
#include <sys/inotify.h>
#include <unistd.h>
#include <cstring>
#include <chrono>
#include <thread>
#include <fcntl.h>
#include "ManualFilesystemWatcher.hpp"


int main()
{
    ManualFilesystemWatcher watcher;

    watcher.watchPath("/home/megaxela/TEST/Hello");
    watcher.watchPath("/home/megaxela/TEST");

    ManualFilesystemWatcher::Event event;

    while (true)
    {
        while (!watcher.receiveFilesystemEvent(event));

        switch (event.action)
        {
        case AbstractFilesystemWatcher::Action::Unknown:
            std::cout << "Unknown";
            break;
        case AbstractFilesystemWatcher::Action::Created:
            std::cout << "Created";
            break;
        case AbstractFilesystemWatcher::Action::Modified:
            std::cout << "Modified";
            break;
        case AbstractFilesystemWatcher::Action::Deleted:
            std::cout << "Deleted";
            break;
        case AbstractFilesystemWatcher::Action::MovedFrom:
            std::cout << "MovedFrom";
            break;
        case AbstractFilesystemWatcher::Action::MovedTo:
            std::cout << "MovedTo";
            break;
        case AbstractFilesystemWatcher::Action::Renamed:
            std::cout << "Renamed";
            break;
        }

        std::cout << " [";

        switch (event.type)
        {
        case AbstractFilesystemWatcher::FileType::Unknown:
            std::cout << "Unknown";
            break;
        case AbstractFilesystemWatcher::FileType::Directory:
            std::cout << "Directory";
            break;
        case AbstractFilesystemWatcher::FileType::File:
            std::cout << "File";
            break;
        }

        std::cout << "] -> ";

        std::cout << event.path << std::endl;
    }
}