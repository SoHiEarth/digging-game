cmake -Bbuild -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=on
cmake --build build --config Debug
echo "Copying files to build directory"
cp -r ./assets/ ./build/assets/
echo "Copying compile_commands.json to root directory"
cp ./build/compile_commands.json ./compile_commands.json
