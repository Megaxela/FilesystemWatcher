#pragma once


#include "BaseFilesystemWatcher.hpp"

/**
 * @brief Class, that describes
 * filesystem watcher with manual path registration.
 */
class ManualFilesystemWatcher : public BaseFilesystemWatcher
{
public:
    /**
     * @brief Constructor.
     */
    ManualFilesystemWatcher();

    /**
     * @brief Method that performs path registration for
     * watching. May throw `std::system_error` exception if
     * something went very wrong.
     * @param path Path to watch.
     */
    void watchPath(const std::string& path);

    /**
     * @brief Method that performs path unregistration from
     * watching. May throw `std::system_error` exception if
     * something went very wrong.
     * @param path Path to remove.
     */
    void unwatchPath(const std::string& path);

    /**
     * @brief Method that allows to receive filesystem event
     * if available.
     * @param event Result event.
     * @return Was result event set.
     */
    bool receiveFilesystemEvent(Event& event);
};

