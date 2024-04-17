#! /bin/bash

if ! [[ -x hgrep ]]; then
    echo "hgrep executable does not exist"
    exit 1
fi

../../tester/run-tests.sh $*



