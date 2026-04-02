# About

This is a simple 1D ray tracer for layered media.

# Conan

    conan install . --build=missing -s build_type=Debug -pr:a=Linux-x86_64-clang-21 --output-folder ./conanBuild
    conan install . --build=missing -s build_type=Release -pr:a=Linux-x86_64-clang-21 --output-folder ./conanBuild
    cmake --preset conan-release -DCLANG_TIDY_EXECUTABLE=/usr/bin/clang-tidy-21
    cmake --build --preset conan-debug
