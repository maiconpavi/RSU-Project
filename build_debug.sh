rm -rd debug/project-libs
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -S . -B debug
cmake --build debug