#!/usr/bin/env bash

PWD=`pwd`
pushd ${PWD}
rm -rf a.out dlconfig ../../lbdex/verilog/dlconfig cpu0.hex *~ libfoobar.cpu0.so *.o *.bc *.s *.S
popd

