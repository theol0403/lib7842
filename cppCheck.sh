#!/bin/bash
cppcheck --enable=all src -Iinclude --suppress=unusedFunction --suppress=missingInclude --max-configs=1 --force --std=c++20 --inconclusive \
    --config-exclude=test/include --config-exclude=include/lib7842/test --config-exclude=include/display --config-exclude=include/pros --suppress=*:*include/lib7842/test\* --suppress=*:*include/pros\* --suppress=*:*include/okapi\* --suppress=*:*test/include\*
