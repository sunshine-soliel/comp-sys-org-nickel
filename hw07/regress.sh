#!/bin/sh
# regress.sh: the HW7 regression test. Runs ./balance on every file in
# inputs/ and compares its output line AND its exit status with the file of
# the same name in expected/.
# Usage, in hw07/ after make:   sh regress.sh
# Exits with 1 if anything differs, so that make test fails.
fail=0
for input in inputs/*.txt; do
    name=$(basename "$input")
    # the output line, then "exit status N" on a second line
    actual=$(./balance < "$input"; echo "exit status $?")
    if [ "$actual" = "$(cat "expected/$name")" ]; then
        echo "PASS  regression $name"
    else
        echo "FAIL  regression $name"
        fail=1
    fi
done
exit $fail
