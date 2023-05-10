 #!/bin/sh -e

_vcpkg=/hdd/Dev/vcpkg/scripts/buildsystems/vcpkg.cmake

cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_MAKE_PROGRAM=ninja -DCMAKE_TOOLCHAIN_FILE=$_vcpkg -G Ninja -S . -B build/release/x86 -DTARGET_32B=True -DVCPKG_TARGET_TRIPLET=x86-linux
cmake --build build/release/x86 --target klhost -j 10
cd build/release/x86
cpack
