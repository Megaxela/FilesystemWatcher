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

    void watchPath(const std::string& path);

    void unwatchPath(const std::string& path);

    bool receiveFilesystemEvent(Event& event);
};

