cd vendor/assimp

cmake CMakeLists.txt
cmake --build . --config Debug
cmake --build . --config Release

cd ../..

premake5 vs2022
PAUSE