#!/bin/bash
cd ../Quake

make clean

make VULKAN_SDK=~/Documents/MoltenVK/Package/Release/MoltenVK

cd ../macOS

rm -f ./Quake.app/Contents/MacOS/vkquake
cp -v ../Quake/vkquake ./Quake.app/Contents/MacOS/
rm -f ./Quake.app/Contents/libs/*
cp -v ../../MoltenVK/Package/Release/MoltenVK/dylib/macOS/libMoltenVK.dylib ./libs/
cp -v /opt/local/lib/libSDL2-2.0.0.dylib ./libs/

echo "---------"

otool -L ./Quake.app/Contents/MacOS/vkquake

echo "---------"

../../macdylibbundler/dylibbundler -of -b -x ./Quake.app/Contents/MacOS/vkquake -d ./Quake.app/Contents/libs -s ./libs

echo "---------"

otool -L ./Quake.app/Contents/MacOS/vkquake

echo "DONE"
