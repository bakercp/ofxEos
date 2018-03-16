#!/usr/bin/env bash

THIS_PATH=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
THIS_NAME=$(basename "${BASH_SOURCE[0]%.*}")
INSTALL_PATH=${THIS_PATH}/${THIS_NAME}
mkdir -p ${INSTALL_PATH}

echo "Installing ${THIS_NAME} data ..."

git clone --depth=1 https://github.com/bakercp/eos.git _tmp

INSTALL_PATH_=${INSTALL_PATH}/share
mkdir -p ${INSTALL_PATH_}
cp -rv _tmp/share/* ${INSTALL_PATH_}/

INSTALL_PATH_=${INSTALL_PATH}/examples/data
mkdir -p ${INSTALL_PATH_}
cp -rv _tmp/examples/data/* ${INSTALL_PATH_}/

rm -rf _tmp/

echo "✅ ${INSTALL_PATH}/${file}"
