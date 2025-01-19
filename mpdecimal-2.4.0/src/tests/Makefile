
SRCDIR = ../libmpdec
INC = -I$(SRCDIR)

CC = gcc
LD = gcc
AR = ar

MPD_WARN = -Wall -W -Wno-unknown-pragmas -std=c99 -pedantic
MPD_CONFIG = -DCONFIG_64 -DASM
MPD_CCOV = @MPD_CCOV@

CONFIGURE_CFLAGS = -Wall -W -Wno-unknown-pragmas -std=c99 -pedantic -DCONFIG_64 -DASM -O2 -fpic
CFLAGS ?= $(CONFIGURE_CFLAGS)


default: runtest

runtest:\
runtest.c $(SRCDIR)/io.h $(SRCDIR)/mpdecimal.h $(SRCDIR)/memory.h \
          $(SRCDIR)/libmpdec.a malloc_fail.c malloc_fail.h mptest.h
	$(CC) $(INC) $(CFLAGS) -o runtest runtest.c malloc_fail.c $(SRCDIR)/libmpdec.a -lm


FORCE:

clean: FORCE
	rm -f *.o *.gch *.gcda *.gcno *.gcov *.dyn *.dpi *.lock runtest

distclean: FORCE
	$(MAKE) clean
	rm -rf Makefile dectest.zip testdata


