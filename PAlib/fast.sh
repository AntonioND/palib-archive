#!/bin/sh
make clean -C lib/arm7
make
make archive
sleep 3600
