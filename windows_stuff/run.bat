cmake -S . -B build/ -DCMAKE_EXPORT_COMPILE_COMMANDS=On -DCMAKE_BUILD_TYPE=Debug
cmake --build build && start ./build/Debug/second_space_mining.exe
