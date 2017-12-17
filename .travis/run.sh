#!/bin/bash

set -e
set -x

if [[ "$(uname -s)" == 'Darwin' ]]; then
    if which pyenv > /dev/null; then
        eval "$(pyenv init -)"
    fi
    pyenv activate conan
fi

if [[ "$CXX_STANDARD" == '' ]]; then
    echo "CXX_STANDARD not set"
    exit 1
fi
python build.py --build missing
