SUBDIRS = .

AM_CPPFLAGS = -I$(HOME)/include -L$(HOME)/lib -std=c++0x

bin_PROGRAMS = genqm
genqm_CFLAGS  = $(GTK_CFLAGS)
genqm_LDADD   = $(GTK_LIBS) $(INTLLIBS) -L$(HOME)/lib  -lxlsreader -liconv -lm
genqm_SOURCES = main.cpp\
				getFileStringUnicode.cpp\
				getFileStringUnicode.h\
				xlsHandler.h\
				xlsHandler.cpp
