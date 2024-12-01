cmake -Bbuild -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build --config Release
