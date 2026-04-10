cd /c/Users/richa/ALTERDUNE/gui
rm -rf build
mkdir build && cd build
cmake -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:/Qt/6.11.0/mingw_64/lib/cmake" ..
mingw32-make -j