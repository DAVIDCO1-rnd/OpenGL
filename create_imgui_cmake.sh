git clone https://github.com/Pesc0/imgui-cmake --recurse-submodules
cd imgui-cmake
git submodule foreach git pull origin HEAD
mkdir build && cd build
cmake -D BUILD_EXAMPLES=ON -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . -j4
./example_sdl_opengl3

