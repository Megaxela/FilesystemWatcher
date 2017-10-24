//
// Created by megaxela on 10/24/17.
//

#pragma once


#include "BaseFilesystemWatcher.hpp"

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

