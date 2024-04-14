#! /bin/bash

if ! [[ -x hzip ]]; then
    echo "hzip executable does not exist"
    exit 1
fi

../../tester/run-tests.sh $*


