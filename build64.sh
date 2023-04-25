 #!/bin/sh -e

_vcpkg=/hdd/Dev/vcpkg/scripts/buildsystems/vcpkg.cmake

cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_MAKE_PROGRAM=ninja -DCMAKE_TOOLCHAIN_FILE=$_vcpkg -G Ninja -S . -B build/release/x64 -DVCPKG_TARGET_TRIPLET=x64-linux
cmake --build build/release/x64 --target klhost -j 10
