#pragma once


#include <unordered_map>
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
    /**
     * @brief Method that performs `inotify_add_watch` call to path.
     * Path may be file or directory. May throw `std::system_error` if
     * watcher was not initialized, `inotify_add_watch` call failed or if it's
     * impossible to get path stat.
     * @param name Path.
     */
    void addToWatchable(const PathType& name) override;

    /**
     * @brief Method that performs receiving events from handler by `read`. And sets
     * first received event to `event` variable, other events will be saved to `m_eventsCacheQueue`.
     * Can throw `std::system_error` if event received, but can't identify path from `wd`.
     * @param event Reference to result event object.
     * @return Was any event got?
     */
    bool receiveEvent(Event& event) override;

    /**
     * @brief Method, that performs removing some path from watching.
     * This method calls `inotify_rm_watch` on path handler if path was found.
     * Can throw `std::system_error` if watcher was not initialized or if `inotify_rm_watch` call failed.
     * @param name
     */
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
    std::unordered_map<HandlerType, PathHandler> m_pathsHandlers;
    std::queue<Event> m_eventsCacheQueue;
};