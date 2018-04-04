#pragma once
#include <string>
#include <fstream>
#include <cstdlib>
#include "filesystem.hpp"

void renameFile(const std::filesystem::path& from, const std::filesystem::path& to)
{
    std::filesystem::rename(from, to);
}

void deleteFile(const std::filesystem::path& dir)
{
    std::filesystem::remove(dir);
}

void createDirectory(const std::filesystem::path& dir)
{
    if (!std::filesystem::create_directory(
        std::filesystem::current_path() / dir
    ))
    {
        std::cerr << "Directory was not created: " << dir << std::endl;
    }
}

bool updateFile(const std::filesystem::path& path)
{
    std::ofstream file(
        std::filesystem::current_path() / path
    );

    if (!file.is_open())
    {
        return false;
    }

    file << "0xDEADBEEF";

    return true;
}

void clearDirectory(const std::filesystem::path& directory)
{
    std::filesystem::remove_all(
        std::filesystem::current_path() / directory
    );
}

void createFile(const std::string& path)
{
    std::ofstream file(path);

    file.close();
}