#!/bin/bash

install_dir=$(readlink -f ../install)

cmake -S ../pprzlinkQt -B build/pprzlinkQt -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$install_dir
cmake --build build/pprzlinkQt
cmake --install build/pprzlinkQt


cmake -S . -B build -DCMAKE_PREFIX_PATH=$install_dir
cmake --build build/

