#!/bin/bash

pacman -Syu --noconfirm && pacman -S --needed --noconfirm \
    mingw-w64-ucrt-x86_64-toolchain \
    mingw-w64-ucrt-x86_64-make \
    mingw-w64-ucrt-x86_64-cmake \
    mingw-w64-ucrt-x86_64-glfw \
    mingw-w64-ucrt-x86_64-assimp \
    mingw-w64-ucrt-x86_64-ffmpeg \
    mingw-w64-ucrt-x86_64-entt \
    mingw-w64-ucrt-x86_64-openal \
    mingw-w64-ucrt-x86_64-freealut \
    mingw-w64-ucrt-x86_64-glm \
    mingw-w64-ucrt-x86_64-stb