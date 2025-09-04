#!/usr/bin/env bash

# SPDX-License-Identifier: GPL-3.0-only

set -eu

# Fail early on non-MacOS systems with a clear message for users/CI
if [ "$(uname)" != "Darwin" ]; then
    echo "ERROR: This package is only supported on macOS and cannot be built here." 1>&2
    echo "Please build on macOS (or use a compatible container/runner)." 1>&2
    exit 1
fi

# https://conda-forge.org/docs/maintainer/knowledge_base.html#newer-c-features-with-old-sdk
# TODO: Is this still needed in macOS 13+ which is the expected?
export CXXFLAGS="${CXXFLAGS} -D_LIBCPP_DISABLE_AVAILABILITY"

python -m pip install . -vvv --no-deps
