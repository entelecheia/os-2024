#! /bin/bash

if ! [[ -x hcat ]]; then
    echo "hcat executable does not exist"
    exit 1
fi

../../tester/run-tests.sh $*


