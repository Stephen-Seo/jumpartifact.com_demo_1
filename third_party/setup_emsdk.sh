#!/usr/bin/env bash

set -ev

SCRIPT_DIR="$(dirname "$0")"

EMSDK_CLONE_DIR="${EMSDK_CLONE_DIR:-"$SCRIPT_DIR/emsdk_clone"}"
EMSDK_TAG_VERSION="${EMSDK_TAG_VERSION:-"4.0.12"}"

if [[ -z "$EMSDK_TAG_VERSION" ]]; then
	echo "ERROR: Invalid EMSDK_TAG_VERSION, empty variable!"
	exit 1
fi

if ! [[ -d "$EMSDK_CLONE_DIR" ]]; then
    git clone https://github.com/emscripten-core/emsdk.git "$EMSDK_CLONE_DIR"
fi

pushd "${EMSDK_CLONE_DIR}"

git pull

./emsdk install "${EMSDK_TAG_VERSION}"
./emsdk activate "${EMSDK_TAG_VERSION}"

popd # EMSDK_CLONE_DIR
