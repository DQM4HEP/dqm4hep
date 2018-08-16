#!/bin/bash

ls -la

source dependencies/root/bin/thisroot.sh

mkdir -p build
cd build

# Disable automatic testing within make on osx. See comment below
# if [[ "${TRAVIS_OS_NAME}" == "osx" ]]; then export DQM4hep_TESTING="OFF";
# else export DQM4HEP_TESTING="ON";
# fi

cmake -DDQM4hep_DOXYGEN_DOC=OFF\
 -DDQM4hep_TESTING=ON \
 -DDQM4hep_WARNING_AS_ERROR=ON \
 -DDQM4hep_DEV_WARNINGS=ON \
 -Dxdrstream_DIR=$TRAVIS_BUILD_DIR/dependencies/xdrstream/lib/cmake \
 ..

if [ $? -ne 0 ]; then
    echo "Failed to run cmake"
    exit 1
fi

make install VERBOSE=1

if [ $? -ne 0 ]; then
    echo "Failed to run make"
    exit 1
fi

ctest --output-on-failure 

if [ $? -ne 0 ]; then
    echo "Failed to run cmake tests"
    exit 1
fi
# 
# # For some reason, running ctest from within make leads to an instantaneous Child aborted exception being thrown on osx. Running the tests manually works without trouble though...
# pkgList="xdrstream DQMCore DQMNet DQMOnline"
# 
# if [[ "${TRAVIS_OS_NAME}" == "osx" ]]; then
#     for pkg in $pkgList
#         do cd $pkg-prefix/src/$pkg-build/
#         ctest -V
# 
#         if [ $? -ne 0 ]; then
#             echo "Failed to run cmake tests"
#             exit 1
#         fi
#         cd -
#     done
# fi


