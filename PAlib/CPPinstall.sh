#!/bin/sh
export CPP=1
make clean
make
make archive
export CPP=0