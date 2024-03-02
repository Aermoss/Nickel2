#pragma once

#include <fstream>
#include <string>

#include "../Renderer/Buffer.hpp"

namespace Nickel2 {
    Nickel2::ext::Buffer readFileBinary(const char* filePath);
    std::string readFile(const char* filePath);
}