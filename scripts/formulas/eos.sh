#! /bin/bash
#
# Eigen
#
# uses CMake

# define the version
VER=0.16.1

# tools for git use
GIT_URL="https://github.com/bakercp/eos.git"
GIT_TAG=$VER

FORMULA_TYPES=( "osx" "linux64")

# download the source code and unpack it into LIB_NAME
function download() {
    git clone --depth=1 ${GIT_URL} eos/
    cd eos/
    git checkout ${GIT_TAG}
}

# prepare the build environment, executed inside the lib src dir
function prepare() {
  echo "Nothing to prepare."
}

# executed inside the lib src dir
function build() {
  echo "Nothing to build."
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {
    echo `pwd`
    # headers
    if [ -d $1/include ]; then
        rm -rf $1/include
    fi

    mkdir -p $1/include/
    cp -r include/* $1/include/

}

# executed inside the lib src dir
function clean() {
  echo "Nothing to clean."
}
