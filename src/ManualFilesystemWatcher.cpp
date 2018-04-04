//
// Created by megaxela on 10/24/17.
//

#include "../include/ManualFilesystemWatcher.hpp"

ManualFilesystemWatcher::ManualFilesystemWatcher()
{

}

void ManualFilesystemWatcher::watchPath(const std::string& path)
{
    addToWatchable(path);
}

void ManualFilesystemWatcher::unwatchPath(const std::string& path)
{
    removeFromWatchable(path);
}

bool ManualFilesystemWatcher::receiveFilesystemEvent(AbstractFilesystemWatcher::Event& event)
{
    return receiveEvent(event);
}
