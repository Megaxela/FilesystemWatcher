
#include <gtest/gtest.h>
#include <ManualFilesystemWatcher.hpp>
#include "Functions.hpp"

const std::filesystem::path sandbox_path = "tests_sandbox";

void pollEvents(ManualFilesystemWatcher& watcher)
{
    ManualFilesystemWatcher::Event event;
    while (watcher.receiveFilesystemEvent(event))
    {
        while (!watcher.receiveFilesystemEvent(event));
    }
}

void checkAsserts(
    ManualFilesystemWatcher& watcher,
    ManualFilesystemWatcher::Action action,
    const std::filesystem::path& path,
    ManualFilesystemWatcher::FileType fileType
)
{
    ManualFilesystemWatcher::Event event;

    ASSERT_TRUE(watcher.receiveFilesystemEvent(event));

    ASSERT_EQ(event.action, action);
    ASSERT_EQ(event.path,   path);
    ASSERT_EQ(event.type,   fileType);

    ASSERT_FALSE(watcher.receiveFilesystemEvent(event));
}

TEST(Manual, FileActions)
{
    ManualFilesystemWatcher watcher;

    ASSERT_NO_THROW(watcher.watchPath(sandbox_path));

    // Testing file

    createFile(sandbox_path / "test_1");

    checkAsserts(
        watcher,
        ManualFilesystemWatcher::Action::Created,
        std::filesystem::current_path() / sandbox_path / "test_1",
        ManualFilesystemWatcher::FileType::File
    );

    updateFile(sandbox_path / "test_1");

    checkAsserts(
        watcher,
        ManualFilesystemWatcher::Action::Modified,
        std::filesystem::current_path() / sandbox_path / "test_1",
        ManualFilesystemWatcher::FileType::File
    );

    renameFile(sandbox_path / "test_1", sandbox_path / "test_2");

    {
        ManualFilesystemWatcher::Event event;

        ASSERT_TRUE(watcher.receiveFilesystemEvent(event));

        ASSERT_EQ(event.action, ManualFilesystemWatcher::Action::MovedFrom);
        ASSERT_EQ(event.path, std::filesystem::current_path() / sandbox_path / "test_1");
        ASSERT_EQ(event.type, ManualFilesystemWatcher::FileType::File);

        ASSERT_TRUE(watcher.receiveFilesystemEvent(event));

        ASSERT_EQ(event.action, ManualFilesystemWatcher::Action::MovedTo);
        ASSERT_EQ(event.path, std::filesystem::current_path() / sandbox_path / "test_2");
        ASSERT_EQ(event.type, ManualFilesystemWatcher::FileType::File);

        ASSERT_FALSE(watcher.receiveFilesystemEvent(event));
    }

    renameFile(sandbox_path / "test_2", sandbox_path / "test_1");

    pollEvents(watcher);

    deleteFile(sandbox_path / "test_1");

    checkAsserts(
        watcher,
        ManualFilesystemWatcher::Action::Deleted,
        std::filesystem::current_path() / sandbox_path / "test_1",
        ManualFilesystemWatcher::FileType::File
    );

    ASSERT_NO_THROW(watcher.unwatchPath(sandbox_path));

}

TEST(Manual, DirectoryActions)
{
    ManualFilesystemWatcher watcher;

    ASSERT_NO_THROW(watcher.watchPath(sandbox_path));

    ManualFilesystemWatcher::Event event;

    // Testing directory

    createDirectory(sandbox_path / "test_dir");

    checkAsserts(
        watcher,
        ManualFilesystemWatcher::Action::Created,
        std::filesystem::current_path() / sandbox_path / "test_dir",
        ManualFilesystemWatcher::FileType::Directory
    );

    createFile(sandbox_path / "test_dir" / "test_file");

    ASSERT_FALSE(watcher.receiveFilesystemEvent(event));

    updateFile(sandbox_path / "test_dir" / "test_file");

    ASSERT_FALSE(watcher.receiveFilesystemEvent(event));
}

void prepareToTesting()
{
    clearDirectory(sandbox_path);
    createDirectory(sandbox_path);
}

int main(int argc, char** argv)
{
    prepareToTesting();

    testing::InitGoogleTest(&argc, argv);

    auto result = RUN_ALL_TESTS();

    clearDirectory(sandbox_path);

    return result;
}