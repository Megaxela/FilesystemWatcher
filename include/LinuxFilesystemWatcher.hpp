//
// Created by megaxela on 10/24/17.
//

#pragma once


#include <map>
#include <queue>
#include "AbstractFilesystemWatcher.hpp"

/**
 * @brief Class, that describes linux specific
 * filesystem watcher implementation.
 */
class LinuxFilesystemWatcher : public AbstractFilesystemWatcher
{
public:

    using PathType = std::string;

    using HandlerType = int;

    /**
     * @brief Constructor.
     * `std::system_error` exception will be thrown
     * if initialization failed.
     */
    LinuxFilesystemWatcher();

    /**
     * @brief Destructor.
     */
    ~LinuxFilesystemWatcher() override;

protected:
    void addToWatchable(const std::string& name) override;

    bool receiveEvent(Event& event) override;

    void removeFromWatchable(const std::string& name) override;

private:
    struct PathHandler
    {
        PathHandler() :
            path(),
            fileType(FileType::Unknown)
        {}

        PathHandler(const PathType &path, FileType fileType) :
            path(path),
            fileType(fileType)
        {}

        PathType path;
        FileType fileType;
    };

    HandlerType m_iNotifyHandler;
    std::map<HandlerType, PathHandler> m_pathsHandlers;
    std::queue<Event> m_eventsCacheQueue;
};

