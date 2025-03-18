#!/bin/bash
set -e

# Step 1: Clean old build and install directories
echo "Cleaning old build and install directories..."

cd ..
if [ -d "build-release" ]; then
    rm -rf build-release
fi

if [ -d "build-debug" ]; then
    rm -rf build-debug
fi

if [ -d "build_mac_release" ]; then
    rm -rf build_mac_release
fi

if [ -d "build_mac_debug" ]; then
    rm -rf build_mac_debug
fi

# Step 2: Build release version
echo "Building Release version..."

mkdir build-release
cd build-release

cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX=../build_mac_release -DCMAKE_OSX_ARCHITECTURES=x86_64 -DENABLE_CPIO=OFF -DENABLE_TAR=OFF -DENABLE_TEST=OFF

make
make install

cd ..

# Step 3: Build debug version
echo "Building Debug version..."

mkdir build-debug
cd build-debug

cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX=../build_mac_debug -DCMAKE_OSX_ARCHITECTURES=x86_64 -DENABLE_CPIO=OFF -DENABLE_TAR=OFF -DENABLE_TEST=OFF
make
make install

cd ..

echo "Build process completed!"