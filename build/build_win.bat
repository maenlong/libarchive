@echo off
setlocal

REM Step 1: Clean old build and install directories
echo Cleaning old build and install directories...

cd ../
if exist build-release (
    rmdir /s /q build-release
)
if exist build-debug (
    rmdir /s /q build-debug
)
if exist build_win_release (
    rmdir /s /q build_win_release
)
if exist build_win_debug (
    rmdir /s /q build_win_debug
)

REM Step 2: Build release version
echo Building Release version...

mkdir build-release
cd build-release

cmake .. -G "Visual Studio 15 2017" -A Win32 -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=../build_win_release
cmake --build . --target ALL_BUILD --config Release
cmake --build . --target INSTALL --config Release

cd ..

REM Step 3: Build debug version
echo Building Debug version...

mkdir build-debug
cd build-debug

cmake .. -G "Visual Studio 15 2017" -A Win32 -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=../build_win_debug -DCMAKE_C_FLAGS="/W0"
cmake --build . --target ALL_BUILD --config Debug
cmake --build . --target INSTALL --config Debug

cd ..

echo Build process completed!
endlocal
