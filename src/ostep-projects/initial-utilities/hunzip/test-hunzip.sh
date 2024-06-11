#! /bin/bash

if ! [[ -x hunzip ]]; then
    echo "hunzip executable does not exist"
    exit 1
fi

../../tester/run-tests.sh $*


