#!/bin/bash

set -e
set -x

git submodule update --init --recursive
git submodule update --remote
pushd third_party/coriander
if [[ ! -d build ]]; then {
    mkdir build
} fi
cd build
if [[ x${CLANG_HOME} != x ]]; then {
    cmake -DCMAKE_BUILD_TYPE=Debug -DCLANG_HOME=${CLANG_HOME} ..
} else {
    cmake -DCMAKE_BUILD_TYPE=Debug ..
} fi
make -j 8

SUDO=sudo
if [[ ! $(cat /proc/1/sched | head -n 1 | grep '[init|systemd]') ]]; then {
    # running in docker
    echo running in docker
    SUDO=
} fi
if [[ $(uname) == Darwin ]]; then {
    SUDO=
} fi

${SUDO} make install

popd
echo Installed coriander
