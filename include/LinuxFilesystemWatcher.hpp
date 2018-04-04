#pragma once


#include <map>
#include <queue>
#include <utility>
#include "AbstractFilesystemWatcher.hpp"

/**
 * @brief Class, that describes linux specific
 * filesystem watcher implementation.
 */
class LinuxFilesystemWatcher : public AbstractFilesystemWatcher
{
public:

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
    void addToWatchable(const PathType& name) override;

    bool receiveEvent(Event& event) override;

    void removeFromWatchable(const PathType& name) override;

private:
    struct PathHandler
    {
        PathHandler() :
            path(),
            fileType(FileType::Unknown)
        {}

        PathHandler(PathType path, FileType fileType) :
            path(std::move(path)),
            fileType(fileType)
        {}

        PathType path;
        FileType fileType;
    };

    HandlerType m_iNotifyHandler;
    std::map<HandlerType, PathHandler> m_pathsHandlers;
    std::queue<Event> m_eventsCacheQueue;
};

