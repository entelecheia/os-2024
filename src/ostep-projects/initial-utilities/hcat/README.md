
In this directory, you should write the program `hcat.c` and compile it into
the binary `hcat` (e.g., `gcc -o hcat hcat.c -Wall -Werror`).

After doing so, you can run the tests from this directory by running the
`test-hcat.sh` script. If all goes well, you will see:

```sh
prompt> ./test-hcat.sh
test 1: passed
test 2: passed
test 3: passed
test 4: passed
test 5: passed
test 6: passed
test 7: passed
prompt>
```

The `test-hcat.sh` script is just a wrapper for the `run-tests.sh` script in
the `tester` directory of this repository. This program has a few options; see
the relevant
[README](https://github.com/remzi-arpacidusseau/ostep-projects/blob/master/tester/README.md)
for details.



