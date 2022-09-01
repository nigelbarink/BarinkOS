#!/bin/bash

cmake -DLLVM_ENABLE_PROJECTS="clang-tools-extra"  -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" ../llvm