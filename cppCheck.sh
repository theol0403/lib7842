#!/bin/bash
cppcheck --enable=all src test/src -Iinclude --suppress=unusedFunction --suppress=missingInclude --max-configs=1 --force --inconclusive \
    --config-exclude=include/display --config-exclude=include/pros --suppress=*:*include/pros\* --suppress=*:*include/okapi\*
