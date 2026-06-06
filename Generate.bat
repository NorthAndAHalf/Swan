set ROOT=%~dp0
cd vendor/assimp
cmake -B build -G "Visual Studio 18 2026" -A x64 ^
    -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY="%ROOT%libs/assimp" ^
    -DCMAKE_RUNTIME_OUTPUT_DIRECTORY="%ROOT%libs/assimp"
cmake --build build --config Debug
cmake --build build --config Release
cd ../..
premake5 vs2026
PAUSE