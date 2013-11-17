#!/bin/sh

#DESTDIR=${DESTDIR:-}
PREFIX=${PREFIX:-/usr}
[ "$DESTDIR" ] && DESTDIR="$DESTDIR/"

./gensflib.py -d $DESTDIR$PREFIX/include/
