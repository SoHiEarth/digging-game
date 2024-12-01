cmake -Bbuild -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build --config Debug
copy /Y ".\build\compile_commands.json" ".\compile_commands.json"
echo "Copying assets folder"
xcopy /Y /E ".\assets" ".\build\assets"
echo "Copying DLLs"
xcopy /Y ".\lib\SDL2\lib\x64\SDL2.dll" ".\build\SDL2.dll"
xcopy /Y ".\lib\SDL2\lib\x64\SDL2_image.dll" ".\build\SDL2_image.dll"
xcopy /Y ".\lib\SDL2\lib\x64\SDL2_ttf.dll" ".\build\SDL2_ttf.dll"
xcopy /Y ".\lib\SDL2\lib\x64\SDL2_mixer.dll" ".\build\SDL2_mixer.dll"
