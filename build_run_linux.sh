#!/bin/sh

cd build3
./premake_linux64 gmake
cd gmake
make -j12
../../bin/App_Dekrispator_gmake_x64_release
