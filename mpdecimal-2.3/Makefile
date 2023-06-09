
# ======================================================================
#                Visual C (nmake) Makefile for libmpdec
# ======================================================================


INSTALLDIR = .
LIBSTATIC = libmpdec-2.3.lib
LIBIMPORT = libmpdec-2.3.dll.lib
LIBSHARED = libmpdec-2.3.dll


OBJS = basearith.obj context.obj constants.obj convolute.obj crt.obj \
       mpdecimal.obj mpsignal.obj difradix2.obj fnt.obj fourstep.obj \
       size.obj memory.obj numbertheory.obj sixstep.obj transpose.obj


MPD_HEADER = mpdecimal_vc.h
MPD_PREC = 9
MPD_DPREC = 18
GMPLIB = "C:\Program Files (x86)\gmp\gmp.lib"
GMPINC = "C:\Program Files (x86)\gmp"


!if "$(MACHINE)" == "x64"
CONFIG = /DCONFIG_64 /DMASM
MPD_PREC = 19
MPD_DPREC = 38
GMPLIB = "C:\Program Files\gmp\gmp.lib"
GMPINC = "C:\Program Files\gmp"
OBJS = $(OBJS) vcdiv64.obj
!endif
!if "$(MACHINE)" == "ansi64"
CONFIG = /DCONFIG_64 /DANSI
MPD_PREC = 19
MPD_DPREC = 38
GMPLIB = "C:\Program Files\gmp\gmp.lib"
GMPINC = "C:\Program Files\gmp"
!endif
!if "$(MACHINE)" == "full_coverage"
CONFIG = /DTEST_COVERAGE /DCONFIG_32 /DANSI
GMPLIB = "C:\Program Files\gmp\gmp.lib"
GMPINC = "C:\Program Files\gmp"
!endif
!if "$(MACHINE)" == "ppro"
CONFIG = /DCONFIG_32 /DPPRO /DMASM
!endif
!if "$(MACHINE)" == "ansi32"
CONFIG = /DCONFIG_32 /DANSI
!endif
!if "$(MACHINE)" == "ansi-legacy"
CONFIG = /DCONFIG_32 /DANSI /DLEGACY_COMPILER
!endif

!if "$(DLL)" == "1"
BFLAGS= /DBUILD_DLL
UFLAGS= /DUSE_DLL
LFLAGS= $(LFLAGS) /DLL
BUILDLIB = $(LIBSHARED)
USELIB = $(LIBIMPORT)
!if "$(DEBUG)" == "1"
OPT = /MDd /Od /Zi
!else
OPT = /MD /Ox /GS /EHsc
!endif
!else
BUILDLIB = $(LIBSTATIC)
USELIB = $(LIBSTATIC)
!if "$(DEBUG)" == "1"
OPT = /MTd /Od /Zi
!else
OPT = /MT /Ox /GS /EHsc
!endif
!endif

CFLAGS = /W3 /D_CRT_SECURE_NO_WARNINGS /nologo $(CONFIG) $(OPT)


default: $(BUILDLIB)


$(LIBSTATIC): Makefile $(OBJS)
	-@if exist $@ del $(LIBSTATIC)
	lib $(LFLAGS) /out:$(LIBSTATIC) $(OBJS)


$(LIBSHARED): Makefile $(OBJS)
	-@if exist $@ del $(LIBSHARED)
	link $(LFLAGS) /out:$(LIBSHARED) /implib:$(LIBIMPORT) $(OBJS)
	mt -manifest $(LIBSHARED).manifest -outputresource:$(LIBSHARED);2


mpdecimal.h:\
Makefile $(MPD_HEADER)
	-@copy /y $(MPD_HEADER) mpdecimal.h
	-@if $(MACHINE)==full_coverage patch < tests\fullcov_header.patch

basearith.obj:\
Makefile basearith.c constants.h mpdecimal.h memory.h typearith.h \
basearith.h
	$(CC) $(BFLAGS) $(CFLAGS) $(PFLAGS) -c basearith.c

constants.obj:\
Makefile constants.c mpdecimal.h constants.h
	$(CC) $(BFLAGS) $(CFLAGS) $(PFLAGS) -c constants.c

context.obj:\
Makefile context.c mpdecimal.h
	$(CC) $(BFLAGS) $(CFLAGS) $(PFLAGS) -c context.c

convolute.obj:\
Makefile convolute.c bits.h mpdecimal.h constants.h fnt.h fourstep.h \
numbertheory.h sixstep.h umodarith.h typearith.h convolute.h vccompat.h
	$(CC) $(BFLAGS) $(CFLAGS) $(PFLAGS) -c convolute.c

crt.obj:\
Makefile crt.c mpdecimal.h numbertheory.h constants.h umodarith.h \
typearith.h crt.h
	$(CC) $(BFLAGS) $(CFLAGS) $(PFLAGS) -c crt.c

difradix2.obj:\
Makefile difradix2.c bits.h mpdecimal.h numbertheory.h constants.h \
umodarith.h typearith.h difradix2.h vccompat.h
	$(CC) $(BFLAGS) $(CFLAGS) $(PFLAGS) -c difradix2.c

fnt.obj:\
Makefile fnt.c bits.h mpdecimal.h difradix2.h numbertheory.h constants.h \
fnt.h vccompat.h
	$(CC) $(BFLAGS) $(CFLAGS) $(PFLAGS) -c fnt.c

fourstep.obj:\
Makefile fourstep.c mpdecimal.h numbertheory.h constants.h sixstep.h \
transpose.h umodarith.h typearith.h fourstep.h
	$(CC) $(BFLAGS) $(CFLAGS) $(PFLAGS) -c fourstep.c

size.obj:\
Makefile size.c bits.h mpdecimal.h constants.h memory.h typearith.h size.h \
vccompat.h
	$(CC) $(BFLAGS) $(CFLAGS) $(PFLAGS) -c size.c

memory.obj:\
Makefile memory.c mpdecimal.h typearith.h memory.h
	$(CC) $(BFLAGS) $(CFLAGS) $(PFLAGS) -c memory.c

mpdecimal.obj:\
Makefile  mpdecimal.c basearith.h mpdecimal.h typearith.h bits.h \
convolute.h crt.h memory.h umodarith.h constants.h mptest.h mptypes.h \
vccompat.h
	$(CC) $(BFLAGS) $(CFLAGS) $(PFLAGS) -c mpdecimal.c

mpsignal.obj:\
Makefile mpdecimal.c mpdecimal.h vccompat.h
	$(CC) $(BFLAGS) $(CFLAGS) $(PFLAGS) -c mpsignal.c

numbertheory.obj:\
Makefile numbertheory.c bits.h mpdecimal.h umodarith.h constants.h \
typearith.h numbertheory.h vccompat.h
	$(CC) $(BFLAGS) $(CFLAGS) $(PFLAGS) -c numbertheory.c

sixstep.obj:\
Makefile sixstep.c bits.h mpdecimal.h difradix2.h numbertheory.h \
constants.h mptypes.h transpose.h umodarith.h typearith.h sixstep.h \
vccompat.h
	$(CC) $(BFLAGS) $(CFLAGS) $(PFLAGS) -c sixstep.c

transpose.obj:\
Makefile transpose.c bits.h mpdecimal.h constants.h typearith.h \
transpose.h vccompat.h
	$(CC) $(BFLAGS) $(CFLAGS) $(PFLAGS) -c transpose.c

vcdiv64.obj:\
Makefile vcdiv64.asm
	ml64 /c /Cx vcdiv64.asm


check:\
$(BUILDLIB)
	-@if exist $(LIBSHARED) copy /y $(LIBSHARED) tests
	-@cd tests
	-@copy /y Makefile.vc Makefile
	-@nmake UFLAGS="$(UFLAGS)" CFLAGS="$(CFLAGS)" USELIB="$(USELIB)"
	-@runshort.bat

extended:\
Makefile $(BUILDLIB)
	-@if exist $(LIBSHARED) copy /y $(LIBSHARED) tests
	-@cd tests
	-@copy /y Makefile.vc Makefile
	-@nmake UFLAGS="$(UFLAGS)" CFLAGS="$(CFLAGS)" USELIB="$(USELIB)" extended

extended_gmp:\
Makefile $(BUILDLIB)
	-@if exist $(LIBSHARED) copy /y $(LIBSHARED) tests
	-@cd tests
	-@copy /y Makefile.vc Makefile
	-@nmake UFLAGS="$(UFLAGS)" CFLAGS="$(CFLAGS)" USELIB="$(USELIB)" GMPINC=$(GMPINC) GMPLIB=$(GMPLIB) extended_gmp

extended_deccheck:\
Makefile $(BUILDLIB)
	-@if exist $(LIBSHARED) copy /y $(LIBSHARED) tests
	-@cd tests
	-@copy /y Makefile.vc Makefile
	-@nmake UFLAGS="$(UFLAGS)" CFLAGS="$(CFLAGS)" USELIB="$(USELIB)" GMPINC=$(GMPINC) GMPLIB=$(GMPLIB) extended_deccheck


FORCE:

bench.exe: Makefile bench.obj
	$(CC) $(UFLAGS) $(CFLAGS) $(PFLAGS) bench.obj $(USELIB)

bench.obj: FORCE
	$(CC) $(UFLAGS) $(CFLAGS) $(PFLAGS) -c bench.c

profile: FORCE
	nmake clean
	nmake "DLL=1" "PFLAGS=/GL" "LFLAGS=/DLL /LTCG:PGI"
	nmake "DLL=1" "PFLAGS=/GL" bench.exe
	bench.exe $(MPD_PREC) 1000
	bench.exe $(MPD_DPREC) 1000
	del /Q *.dll bench.exe
	link /DLL /LTCG:PGO /out:$(LIBSHARED) /implib:$(LIBIMPORT) $(OBJS)
	mt -manifest $(LIBSHARED).manifest -outputresource:$(LIBSHARED);2
	$(CC) $(UFLAGS) $(CFLAGS) $(PFLAGS) bench.c $(USELIB)
	bench.exe $(MPD_PREC) 1000
	bench.exe $(MPD_DPREC) 1000

clean: FORCE
	-@if exist *.obj del *.obj
	-@if exist *.dll del *.dll
	-@if exist *.exp del *.exp
	-@if exist *.lib del *.lib
	-@if exist *.ilk del *.ilk
	-@if exist *.pdb del *.pdb
	-@if exist *.pgc del *.pgc
	-@if exist *.pgd del *.pgd
	-@if exist *.manifest del *.manifest
	-@if exist *.exe del *.exe
	-@if exist mpdecimal.h del mpdecimal.h
	-@if exist build rd /q /s build
	-@if exist python\cdecimal.pyd del python\cdecimal.pyd
	-@if exist python\cdecimal_d.pyd del python\cdecimal_d.pyd
	-@if exist python\*.pyc del python\*.pyc
	-@cd tests && copy /y Makefile.vc Makefile && nmake clean

distclean: FORCE
	nmake clean
	-@if exist Makefile del Makefile
	-@if exist python\decimaltestdata rd /q /s python\decimaltestdata
	-@if exist python\__pycache__ rd /q /s python\__pycache__
	-@cd tests && nmake distclean

