#include "utils.hpp"

namespace nickel2 {
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