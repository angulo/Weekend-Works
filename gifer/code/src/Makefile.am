AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}
AM_CPPFLAGS = $(GIFER_CFLAGS) -I . -std=c++0x -DPROGRAMNAME_LOCALEDIR=\"${PROGRAMNAME_LOCALEDIR}\"

bin_PROGRAMS = gifer
bindir = /usr/bin

gifer_SOURCES = gifer.h gifer.cpp
gifer_LDADD = $(GIFER_LIBS) 
