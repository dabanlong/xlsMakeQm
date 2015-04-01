SUBDIRS = .

AM_CPPFLAGS = -I$(HOME)/include -I$(PWD)/libxls-0.2.0/include -L$(HOME)/lib -std=c++0x

bin_PROGRAMS = genqm
genqm_CFLAGS  = $(GTK_CFLAGS)
genqm_LDADD   = $(GTK_LIBS) $(INTLLIBS) -L$(HOME)/lib -lxlsreader -lm
genqm_SOURCES = main.cpp\
				ParseGuiString.cpp\
				ParseGuiString.h\
				xlsHandler.h\
				xlsHandler.cpp
