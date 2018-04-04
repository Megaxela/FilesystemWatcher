#pragma once

#include <string>
#include <functional>

/**
 * @brief Class, that describes abstract
 * filesystem watcher.
 */
class AbstractFilesystemWatcher
{
public:

    using PathType = std::string;

    /**
     * @brief Enum class for
     * path types.
     */
    enum class FileType
    {
        Unknown,   //< Service value.
        Directory, //< Path points directory.
        File       //< Path points file.
    };

    /**
     * @brief Enum class for
     * action that was happened with
     * file or directory.
     */
    enum class Action
    {
        Unknown,   //< Service value.
        Created,   //< New file or directory was detected.
        Modified,  //< Existing file was modificated.
        Deleted,   //< File was deleted.
        MovedFrom, //< File was moved from watchable directory.
        MovedTo,   //< File was moved to watchable directory.
    };

    /**
     * @brief Filesystem event.
     */
    struct Event
    {
        /**
         * @brief Constructor.
         */
        Event() :
            path(),
            type(FileType::Unknown),
            action(Action ::Unknown)
        {}

        /**
         * @brief Constructor.
         * @param path Full path to file.
         * @param type File type.
         * @param action Action that happened
         * with file or directory.
         */
        Event(const PathType& path,
              FileType type,
              Action action) :
            path(path),
            type(type),
            action(action)
        {}

        PathType path;
        FileType type;
        Action action;
    };

    /**
     * @brief Constructor.
     */
    AbstractFilesystemWatcher() = default;

    /**
     * @brief Destructor.
     */
    virtual ~AbstractFilesystemWatcher() = default;

    // Disable copying
    AbstractFilesystemWatcher(const AbstractFilesystemWatcher&) = delete;
    AbstractFilesystemWatcher& operator=(const AbstractFilesystemWatcher&) = delete;

protected:

    /**
     * @brief Method for adding path to watch for.
     * @param name Path to directory or file.
     */
    virtual void addToWatchable(const PathType& name) = 0;

    /**
     * @brief Method for removing path from watching list.
     * @param name
     */
    virtual void removeFromWatchable(const PathType& name) = 0;

    /**
     * @brief Method for receiving event of some filesystem
     * changes.
     * @param event Out event.
     * @return Is it was some event ready?
     */
    virtual bool receiveEvent(Event& event) = 0;
};

