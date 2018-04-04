#include "LinuxFilesystemWatcher.hpp"
#include <sys/inotify.h>
#include <unistd.h>
#include <system_error>
#include <cstring>
#include <linux/limits.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUF_LEN (1024 * (sizeof(inotify_event) + 16))

LinuxFilesystemWatcher::LinuxFilesystemWatcher() :
    m_iNotifyHandler(-1),
    m_pathsHandlers()
{
    m_iNotifyHandler = inotify_init();

    if (m_iNotifyHandler < 0)
    {
        throw std::system_error(
            errno,
            std::system_category(),
            strerror(errno)
        );
    }

    // Setting nonblocking mode
    fcntl(
        m_iNotifyHandler,
        F_SETFL,
        fcntl(
            m_iNotifyHandler,
            F_GETFL
        ) | O_NONBLOCK
    );
}

LinuxFilesystemWatcher::~LinuxFilesystemWatcher()
{
    if (m_iNotifyHandler >= 0)
    {
        // Closing handlers
        for (auto&& element : m_pathsHandlers)
        {
            inotify_rm_watch(m_iNotifyHandler, element.first);
        }

        m_pathsHandlers.clear();

        close(m_iNotifyHandler);
        m_iNotifyHandler = -1;
    }
}

void LinuxFilesystemWatcher::addToWatchable(const std::string& name)
{
    if (m_iNotifyHandler < 0)
    {
        throw std::system_error(
            0xDEADBEEF,
            std::system_category(),
            "Filesystem watcher was not initialized."
        );
    }

    HandlerType watchHandle = inotify_add_watch(
        m_iNotifyHandler,
        name.c_str(),
        IN_MODIFY | IN_CREATE | IN_DELETE | IN_MOVE
    );

    if (watchHandle < 0)
    {
        throw std::system_error(
            errno,
            std::system_category(),
            strerror(errno)
        );
    }

    PathHandler handler;

    char actualPath[PATH_MAX + 1];

    handler.path = realpath(name.c_str(), actualPath);

    struct stat s{};
    if (stat(actualPath, &s) == 0)
    {
        if (s.st_mode & S_IFDIR)
        {
            handler.fileType = FileType::Directory;
        }
        else if (s.st_mode & S_IFREG)
        {
            handler.fileType = FileType::File;
        }
    }
    else
    {
        throw std::system_error(
            errno,
            std::system_category(),
            strerror(errno)
        );
    }


    m_pathsHandlers[watchHandle] = handler;
}

static uint8_t buffer[BUF_LEN];

bool LinuxFilesystemWatcher::receiveEvent(AbstractFilesystemWatcher::Event& outEvent)
{
    if (m_iNotifyHandler < 0)
    {
        throw std::system_error(
            0xDEADBEEF,
            std::system_category(),
            "Filesystem watcher was not initialized."
        );
    }

    // Checking cache queue for events
    if (!m_eventsCacheQueue.empty())
    {
        outEvent = m_eventsCacheQueue.front();
        m_eventsCacheQueue.pop();
        return true;
    }

    auto length = read(m_iNotifyHandler, buffer, BUF_LEN);

    if (length < 0)
    {
        if (errno == EAGAIN)
        {
            return false;
        }
        else
        {
            throw std::system_error(
                errno,
                std::system_category(),
                strerror(errno)
            );
        }
    }

    int i = 0;

    bool result = false;

    while (i < length)
    {
        auto* event = (inotify_event*) &buffer[i];

        auto element = m_pathsHandlers.find(event->wd);
        if (element == m_pathsHandlers.end())
        {
            throw std::system_error(
                0xDEADBEEF,
                std::generic_category(),
                "Unknown `wd` in event. " + std::to_string(event->wd)
            );
        }


        Event newEvent;

        if (element->second.fileType == FileType::File)
        {
            newEvent.path = element->second.path;
        }
        else if (event->len > 0)
        {
            newEvent.path = element->second.path;
            newEvent.path += "/";
            newEvent.path += event->name;
        }
        else
        {
            // What???
        }

        if (event->mask & IN_ISDIR)
        {
            newEvent.type = FileType::Directory;
        }
        else
        {
            newEvent.type = FileType::File;
        }

        if (event->mask & IN_CREATE)
        {
            newEvent.action = Action::Created;
        }
        else if (event->mask & IN_DELETE)
        {
            newEvent.action = Action::Deleted;
        }
        else if (event->mask & IN_MODIFY)
        {
            newEvent.action = Action::Modified;
        }
        else if (event->mask & IN_MOVED_FROM)
        {
            newEvent.action = Action::MovedFrom;
        }
        else if (event->mask & IN_MOVED_TO)
        {
            newEvent.action = Action::MovedTo;
        }

        if (!result)
        {
            result = true;
            outEvent = newEvent;
        }
        else
        {
            m_eventsCacheQueue.push(newEvent);
        }

        i += sizeof(inotify_event) + event->len;
    }

    return result;
}

void LinuxFilesystemWatcher::removeFromWatchable(const std::string& name)
{
    std::string realPath;
    {
        char actualPath[PATH_MAX + 1];

        realPath = realpath(name.c_str(), actualPath);
    }


    auto findResult = std::find_if(
        m_pathsHandlers.begin(),
        m_pathsHandlers.end(),
        [=](const std::pair<HandlerType, PathHandler>& handler)
        {
            return handler.second.path == realPath;
        }
    );

    if (findResult == m_pathsHandlers.end())
    {
        return;
    }

    m_pathsHandlers.erase(findResult->first);
}
