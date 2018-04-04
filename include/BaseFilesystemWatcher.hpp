//
// Created by megaxela on 10/24/17.
//

#pragma once

#ifdef OS_WINDOWS
    #error Not implemented.
#endif
#ifdef OS_LINUX
    #include "LinuxFilesystemWatcher.hpp"

    #define __SPECIFIC_FSW LinuxFilesystemWatcher
#endif
#ifdef OS_OSX
    #error Not implemented.
#endif

#ifndef __SPECIFIC_FSW
    #error Filesystem watcher implementation is not founded.
#endif

/**
 * @brief Class, that describes basic
 * base filesystem watcher.
 */
class BaseFilesystemWatcher : public __SPECIFIC_FSW
{};

