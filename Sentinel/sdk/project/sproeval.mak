# Microsoft Developer Studio Generated NMAKE File, Based on sproeval.dsp
!IF "$(CFG)" == ""
CFG=sproeval - Win32 MDRelease
!MESSAGE No configuration specified. Defaulting to sproeval - Win32 MDRelease.
!ENDIF 

!IF "$(CFG)" != "sproeval - Win32 MDDebug" && "$(CFG)" != "sproeval - Win32 MTDebug" && "$(CFG)" != "sproeval - Win32 MTRelease" && "$(CFG)" != "sproeval - Win32 MDRelease"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "sproeval.mak" CFG="sproeval - Win32 MDRelease"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "sproeval - Win32 MDDebug" (based on "Win32 (x86) Application")
!MESSAGE "sproeval - Win32 MTDebug" (based on "Win32 (x86) Application")
!MESSAGE "sproeval - Win32 MTRelease" (based on "Win32 (x86) Application")
!MESSAGE "sproeval - Win32 MDRelease" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "sproeval - Win32 MDDebug"

OUTDIR=.\./MDDebug
INTDIR=.\./MDDebug
# Begin Custom Macros
OutDir=.\./MDDebug
# End Custom Macros

ALL : "$(OUTDIR)\sproeval.exe"


CLEAN :
	-@erase "$(INTDIR)\Sproeval.obj"
	-@erase "$(INTDIR)\Sproeval.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\sproeval.exe"
	-@erase "$(OUTDIR)\sproeval.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=df.exe
F90_PROJ=/include:"$(INTDIR)\\" /module:"./MDDebug/" /object:"./MDDebug/" 
F90_OBJS=.\./MDDebug/

.SUFFIXES: .fpp

.for{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

.f{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

.f90{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

.fpp{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\sproeval.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Sproeval.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\sproeval.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib ./MD/spromeps.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\sproeval.pdb" /debug /machine:I386 /out:"$(OUTDIR)\sproeval.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Sproeval.obj" \
	"$(INTDIR)\Sproeval.res"

"$(OUTDIR)\sproeval.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "sproeval - Win32 MTDebug"

OUTDIR=.\./MTDebug
INTDIR=.\./MTDebug
# Begin Custom Macros
OutDir=.\./MTDebug
# End Custom Macros

ALL : "$(OUTDIR)\sproeval.exe"


CLEAN :
	-@erase "$(INTDIR)\Sproeval.obj"
	-@erase "$(INTDIR)\Sproeval.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\sproeval.exe"
	-@erase "$(OUTDIR)\sproeval.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=df.exe
F90_PROJ=/include:"$(INTDIR)\\" /module:"./MTDebug/" /object:"./MTDebug/" 
F90_OBJS=.\./MTDebug/

.SUFFIXES: .fpp

.for{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

.f{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

.f90{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

.fpp{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\sproeval.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Sproeval.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\sproeval.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib ./MT/spromeps.lib MSVCRT.lib LIBCMT.LIB /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\sproeval.pdb" /debug /machine:I386 /out:"$(OUTDIR)\sproeval.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Sproeval.obj" \
	"$(INTDIR)\Sproeval.res"

"$(OUTDIR)\sproeval.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "sproeval - Win32 MTRelease"

OUTDIR=.\./MTRelease
INTDIR=.\./MTRelease
# Begin Custom Macros
OutDir=.\./MTRelease
# End Custom Macros

ALL : "$(OUTDIR)\sproeval.exe"


CLEAN :
	-@erase "$(INTDIR)\Sproeval.obj"
	-@erase "$(INTDIR)\Sproeval.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\sproeval.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=df.exe
F90_PROJ=/include:"$(INTDIR)\\" /module:"./MTRelease/" /object:"./MTRelease/" 
F90_OBJS=.\./MTRelease/

.SUFFIXES: .fpp

.for{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

.f{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

.f90{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

.fpp{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_NET_APIS" /Fp"$(INTDIR)\sproeval.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Sproeval.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\sproeval.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib ./MT/spromeps.lib MSVCRT.lib LIBCMT.LIB /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\sproeval.pdb" /machine:I386 /out:"$(OUTDIR)\sproeval.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Sproeval.obj" \
	"$(INTDIR)\Sproeval.res"

"$(OUTDIR)\sproeval.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "sproeval - Win32 MDRelease"

OUTDIR=.\./MDRelease
INTDIR=.\./MDRelease
# Begin Custom Macros
OutDir=.\./MDRelease
# End Custom Macros

ALL : "$(OUTDIR)\sproeval.exe"


CLEAN :
	-@erase "$(INTDIR)\Sproeval.obj"
	-@erase "$(INTDIR)\Sproeval.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\sproeval.exe"
	-@erase "$(OUTDIR)\sproeval.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=df.exe
F90_PROJ=/include:"$(INTDIR)\\" /module:"./MDRelease/" /object:"./MDRelease/" 
F90_OBJS=.\./MDRelease/

.SUFFIXES: .fpp

.for{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

.f{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

.f90{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

.fpp{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\sproeval.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Sproeval.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\sproeval.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib ./MD/spromeps.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\sproeval.pdb" /debug /machine:I386 /out:"$(OUTDIR)\sproeval.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Sproeval.obj" \
	"$(INTDIR)\Sproeval.res"

"$(OUTDIR)\sproeval.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("sproeval.dep")
!INCLUDE "sproeval.dep"
!ELSE 
!MESSAGE Warning: cannot find "sproeval.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "sproeval - Win32 MDDebug" || "$(CFG)" == "sproeval - Win32 MTDebug" || "$(CFG)" == "sproeval - Win32 MTRelease" || "$(CFG)" == "sproeval - Win32 MDRelease"
SOURCE=.\Sproeval.c

"$(INTDIR)\Sproeval.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Sproeval.rc

"$(INTDIR)\Sproeval.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

