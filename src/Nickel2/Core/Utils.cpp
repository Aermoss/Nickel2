#include <Nickel2/nkpch.hpp>
#include <Nickel2/Core/Utils.hpp>

namespace Nickel2 {
    Nickel2::ext::Buffer readFileBinary(const char* filePath) {
        std::ifstream stream(filePath, std::ios::binary | std::ios::ate);
        if (!stream) { return {}; }
        std::streampos end = stream.tellg();
        stream.seekg(0, std::ios::beg);
        uint64_t size = end - stream.tellg();
        if (size == 0) { return {}; }
        Nickel2::ext::Buffer buffer(size);
        stream.read(buffer.as<char>(), size);
        stream.close();
        return buffer;
    }

    std::string readFile(const char* filePath) {
        std::ifstream file(filePath, std::ios::binary);
        std::string contents;
        file.seekg(0, std::ios::end);
        contents.resize(file.tellg());
        file.seekg(0, std::ios::beg);
        file.read(&contents[0], contents.size());
        file.close();
        return contents;
    }
}