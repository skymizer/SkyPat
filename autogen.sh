#!/bin/sh
#                    The pat Team 
#

LIBTOOLIZE=libtoolize
my_uname=`uname`

if [ ${my_uname} = "Darwin" ]; then
  LIBTOOLIZE=glibtoolize
fi
root=`dirname $0`
cd $root

aclocal -I ./m4
autoheader
${LIBTOOLIZE} --force
automake --add-missing
autoconf
rm -rf include/pat/Config/Config.h.in~
