#!/bin/sh

#DESTDIR=${DESTDIR:-}
PREFIX=${PREFIX:-/usr}
[ "$DESTDIR" ] && DESTDIR="$DESTDIR/"

umask 022
./gensflib.py -d $DESTDIR$PREFIX/include/
