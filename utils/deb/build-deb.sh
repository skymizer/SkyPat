#!/bin/sh

cd `dirname "$0"`/../../
./autogen.sh
dpkg-buildpackage -rfakeroot
