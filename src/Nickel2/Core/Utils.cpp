#include <Nickel2/nkpch.hpp>
#include <Nickel2/Core/Utils.hpp>

namespace Nickel2::Utils {
    bool IsFileExists(const char* filePath) {
        std::ifstream file(filePath);
        return file.good();
    }

    void WriteFileBinary(const char* filePath, Buffer& buffer) {
        std::ofstream file(filePath, std::ios::out | std::ios::binary);
        if (!file.is_open()) return;
        file.write(buffer.as<char>(), buffer.size);
        file.close();
        return;
    }

    void WriteFileBinary(const char* filePath, const std::vector<uint32_t>& data) {
        std::ofstream file(filePath, std::ios::out | std::ios::binary);
        if (!file.is_open()) return;
        file.write((char*) data.data(), data.size() * sizeof(uint32_t));
        file.close();
        return;
    }

    void WriteFile(const char* filePath, const std::string& data) {
        std::ofstream file(filePath, std::ios::out | std::ios::binary);
        if (!file.is_open()) return;
        file.write(&data[0], data.size());
        file.close();
        return;
    }

    Buffer ReadFileBinary(const char* filePath) {
        std::ifstream file(filePath, std::ios::in | std::ios::binary);
        if (!file.is_open()) return {};
        file.seekg(0, std::ios::end);
        Buffer buffer(file.tellg());
        file.seekg(0, std::ios::beg);
        file.read(buffer.as<char>(), buffer.size);
        file.close();
        return buffer;
    }

    void ReadFileBinary(const char* filePath, std::vector<uint32_t>& data) {
        std::ifstream file(filePath, std::ios::in | std::ios::binary);
        if (!file.is_open()) return;
        file.seekg(0, std::ios::end);
        data.resize(file.tellg() / sizeof(uint32_t));
        file.seekg(0, std::ios::beg);
        file.read((char*) data.data(), data.size() * sizeof(uint32_t));
        file.close();
        return;
    }

    std::string ReadFile(const char* filePath) {
        std::ifstream file(filePath, std::ios::in | std::ios::binary);
        std::string data;
        if (!file.is_open()) return data;
        file.seekg(0, std::ios::end);
        data.resize(file.tellg());
        file.seekg(0, std::ios::beg);
        file.read(&data[0], data.size());
        file.close();
        return data;
    }
}