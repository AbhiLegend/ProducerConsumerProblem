#!/bin/bash
source /opt/intel/oneapi/setvars.sh > /dev/null 2>&1
/bin/echo "##" $(whoami) is compiling SYCL Code -- oneAPI Hack2Skill example - 1 of 1 hellw.cpp
icpx -fsycl lab/hellw.cpp
if [ $? -eq 0 ]; then ./a.out; fi

