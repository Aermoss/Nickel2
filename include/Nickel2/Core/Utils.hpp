#pragma once

#include <fstream>
#include <string>

#include "Buffer.hpp"

namespace Nickel2::Utils {
    bool IsFileExists(const char* filePath);

    void WriteFileBinary(const char* filePath, Buffer& data);
    void WriteFileBinary(const char* filePath, const std::vector<uint32_t>& data);
    void WriteFile(const char* filePath, const std::string& data);

    Buffer ReadFileBinary(const char* filePath);
    void ReadFileBinary(const char* filePath, std::vector<uint32_t>& data);
    std::string ReadFile(const char* filePath);
}