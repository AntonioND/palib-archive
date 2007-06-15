#!/bin/sh
cd $PAPATH
make clean
make
make archive
sleep 3600
