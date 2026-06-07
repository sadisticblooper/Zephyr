# Discord

My Server server for those who want to discuss the development of this library and offet suggestions.
https://discord.gg/8j7AsuEEmG

# Zephyr

A C++20 library for reading, writing, and modding Unreal Engine pak files.

## Requirements

- CMake 3.21 or later
- A C++20 capable compiler (MSVC 2022, GCC 12, Clang 14)

## Building

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

To include Oodle decompression support:

```bash
cmake -B build -DZEPHYR_WITH_OODLE=ON
```

## Usage

Link against the `Zephyr` static library and include the public headers:

```cmake
target_link_libraries(your_target PRIVATE Zephyr)
target_include_directories(your_target PRIVATE path/to/Zephyr/include)
```

## License

See [LICENSE](LICENSE).
