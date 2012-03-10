#!/bin/bash

base=`readlink -mn $0`
basedir=`dirname $base`
basename=`basename $basedir`
debname=$basename.deb

if [ ! -z $1 ]; then
  debname=$1.deb
fi

if [ ! -f "bin/msnake" ]; then
  echo "msnake binary not found, thus let's compile!"
  cmake .
  make
fi

mkdir -p package/usr/bin/
cp -rf DEBIAN package
cp -f bin/msnake package/usr/bin

#cd $basedir/..

echo -n "Creating $debname ..."
dpkg-deb -z8 -Zgzip --build package > /dev/null
mv -f package.deb $basedir/$debname
rm -rf package

echo " Done!"
