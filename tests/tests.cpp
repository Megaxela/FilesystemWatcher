
#include <gtest/gtest.h>
#include <ManualFilesystemWatcher.hpp>
#include "Functions.hpp"

const std::filesystem::path sandbox_path = "tests_sandbox";

#define TEST_SANBOX_PATH sandbox_path / test_info_->test_case_name() / test_info_->name();

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
    auto test_sandbox_path = TEST_SANBOX_PATH;

    ManualFilesystemWatcher watcher;

    ASSERT_NO_THROW(watcher.watchPath(test_sandbox_path));

    createFile(test_sandbox_path / "test_1");

    checkAsserts(
        watcher,
        ManualFilesystemWatcher::Action::Created,
        std::filesystem::current_path() / test_sandbox_path / "test_1",
        ManualFilesystemWatcher::FileType::File
    );

    updateFile(test_sandbox_path / "test_1");

    checkAsserts(
        watcher,
        ManualFilesystemWatcher::Action::Modified,
        std::filesystem::current_path() / test_sandbox_path / "test_1",
        ManualFilesystemWatcher::FileType::File
    );

    renameFile(test_sandbox_path / "test_1", test_sandbox_path / "test_2");

    {
        ManualFilesystemWatcher::Event event;

        ASSERT_TRUE(watcher.receiveFilesystemEvent(event));

        ASSERT_EQ(event.action, ManualFilesystemWatcher::Action::MovedFrom);
        ASSERT_EQ(event.path, std::filesystem::current_path() / test_sandbox_path / "test_1");
        ASSERT_EQ(event.type, ManualFilesystemWatcher::FileType::File);

        ASSERT_TRUE(watcher.receiveFilesystemEvent(event));

        ASSERT_EQ(event.action, ManualFilesystemWatcher::Action::MovedTo);
        ASSERT_EQ(event.path, std::filesystem::current_path() / test_sandbox_path / "test_2");
        ASSERT_EQ(event.type, ManualFilesystemWatcher::FileType::File);

        ASSERT_FALSE(watcher.receiveFilesystemEvent(event));
    }

    renameFile(test_sandbox_path / "test_2", test_sandbox_path / "test_1");

    pollEvents(watcher);

    deleteFile(test_sandbox_path / "test_1");

    checkAsserts(
        watcher,
        ManualFilesystemWatcher::Action::Deleted,
        std::filesystem::current_path() / test_sandbox_path / "test_1",
        ManualFilesystemWatcher::FileType::File
    );

    ASSERT_NO_THROW(watcher.unwatchPath(test_sandbox_path));

}

TEST(Manual, NoManualRecurseWatch)
{
    auto test_sandbox_path = TEST_SANBOX_PATH;

    ManualFilesystemWatcher watcher;

    ASSERT_NO_THROW(watcher.watchPath(test_sandbox_path));

    ManualFilesystemWatcher::Event event;

    createDirectory(test_sandbox_path / "recurse");

    // Testing directory
    checkAsserts(
        watcher,
        ManualFilesystemWatcher::Action::Created,
        std::filesystem::current_path() / test_sandbox_path / "recurse",
        ManualFilesystemWatcher::FileType::Directory
    );

    createFile(test_sandbox_path / "recurse" / "test_file");

    ASSERT_FALSE(watcher.receiveFilesystemEvent(event));

    updateFile(test_sandbox_path / "recurse" / "test_file");

    ASSERT_FALSE(watcher.receiveFilesystemEvent(event));
}

TEST(Manual, RemovePath)
{
    auto test_sandbox_path = TEST_SANBOX_PATH;

    ManualFilesystemWatcher watcher;

    ASSERT_NO_THROW(watcher.watchPath(test_sandbox_path));

    createFile(test_sandbox_path / "some_file");

    checkAsserts(
        watcher,
        ManualFilesystemWatcher::Action::Created,
        std::filesystem::current_path() / test_sandbox_path / "some_file",
        ManualFilesystemWatcher::FileType::File
    );

    ASSERT_NO_THROW(watcher.unwatchPath(test_sandbox_path));

    createFile(test_sandbox_path / "another_file");

    ManualFilesystemWatcher::Event event;

    ASSERT_FALSE(watcher.receiveFilesystemEvent(event));

//    ASSERT_NO_THROW(watcher.watchPath())
}

void prepareToTesting()
{
    clearDirectory(sandbox_path);
    createDirectory(sandbox_path);

    auto unitTests = ::testing::UnitTest::GetInstance();

    // Creating tests sandboxes
    auto testCaseCount = unitTests->total_test_case_count();

    for (auto testCaseIndex = 0;
         testCaseIndex < testCaseCount;
         ++testCaseIndex)
    {
        auto testCase = unitTests->GetTestCase(testCaseIndex);

        auto testCount = testCase->total_test_count();

        createDirectory(sandbox_path / testCase->name());

        for (auto testIndex = 0;
             testIndex < testCount;
             ++testIndex)
        {
            createDirectory(sandbox_path / testCase->name() / testCase->GetTestInfo(testIndex)->name());
        }
    }
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);

    prepareToTesting();

    auto result = RUN_ALL_TESTS();

    clearDirectory(sandbox_path);

    return result;
}