#!/bin/bash
for I in $(find include/lib7842/ -type d); do
    DIR=$DIR" -I$I"
done

for I in $(find test/include/ -type d); do
    DIR=$DIR" -I$I"
done

for I in $(find include/okapi/ -type d); do
    DIR=$DIR" -I$I"
done

for I in $(find include/pros/ -type d); do
    DIR=$DIR" -I$I"
done

cppcheck --enable=all src test/src $DIR --suppress=unusedFunction --suppress=missingInclude