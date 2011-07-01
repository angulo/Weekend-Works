#! /bin/bash
intltoolize --copy --automake --force
aclocal
autoheader
autoconf 
automake --add-missing
