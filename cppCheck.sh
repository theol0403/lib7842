#!/bin/bash
cppcheck --enable=all src test/src -Iinclude --suppress=unusedFunction --suppress=missingInclude
