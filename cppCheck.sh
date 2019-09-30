#!/bin/bash
for I in $(find include/lib7842/ -type d); do
    DIR=$DIR" -I$I"
done

cppcheck --enable=all src test/src $DIR --suppress=unusedFunction --suppress=missingInclude