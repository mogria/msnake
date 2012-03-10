#!/bin/bash

base=`readlink -mn $0`
basedir=`dirname $base`
basename=`basename $basedir`

if [ ! -f "bin/msnake" ]; then
  echo "msnake binary not found, thus let's compile!"
  cmake .
  make
fi

mkdir -p usr/bin/
cp -f bin/msnake usr/bin

cd $basedir/..

dpkg-deb -z8 -Zgzip --build $basename
cp -f $basename.deb $basedir

echo "Created msnake.deb!"
