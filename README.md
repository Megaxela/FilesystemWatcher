# Cross Platform Filesystem Watcher
It's C++11 based cross platform filesystem watcher.

## OS Covering
:heavy_check_mark: Linux

:x: Windows

:x: MacOS

## Build
It's CMake based project, it can be used as
subproject (`add_subdirectory`) in your CMake project.

But also you can build it and use in any build system.

Steps to build it:
1. Clone repo: `git clone https://github.com/Megaxela/FilesystemWatcher`
1. Go into cloned repo: `cd FilesystemWatcher`
1. Create build folder `mkdir build`
1. Go into build folder `cd build`
1. Setup project: `cmake ..`
1. You may build tests and examples:
    1. To build tests, you have to add `-DFSWATCHER_BUILD_TESTS=On` key (but tests are using C++17 standard)
    1. To build examples, you have to add `-DFSWATCHER_BUILD_EXAMPLES=On`.
1. Build library: `cmake --build` or `make`

## Usage example

```cpp
#include <ManualFilesystemWatcher.hpp>
#include <iostream>

int main(int argc, char** argv)
{
    // Create watcher object
    ManualFilesystemWatcher watcher;

    // Add directory to watch

    try
    {
        watcher.watchPath("some_path");
    }
    catch (std::system_error& e)
    {
        std::cerr << "Can't add path to watcher. Error: " << e.what() << std::endl;
        return 1;
    }

    // Declare event object
    ManualFilesystemWatcher::Event event;

    // Do something with path elements

    // Get event
    if (!watcher.receiveFilesystemEvent(event))
    {
        std::cout << "There is nothing happen." << std::endl;
        return 0;
    }

    std::cout << "Path of changed obj: " << event.path << std::endl;

    // Continue getting events.

    // Unregister path from watcher (it's not throwing any exceptions)
    watcher.unwatchPath("some_path");

    return 0;
}
```

## LICENSE

<img align="right" src="http://opensource.org/trademarks/opensource/OSI-Approved-License-100x137.png">

Library is licensed under the [MIT License](https://opensource.org/licenses/MIT)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.