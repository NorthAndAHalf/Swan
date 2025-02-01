cd vendor/assimp

cmake CMakeLists.txt
cmake --build .
cd ../..

premake5 vs2022
PAUSE