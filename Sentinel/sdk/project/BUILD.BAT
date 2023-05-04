@ECHO OFF
:: -----------------------------------------------------------------------------
:: (C) Copyright 1991-2002 Rainbow Technologies, Inc. All rights reserved
:: -----------------------------------------------------------------------------

REM ************************************************************************
REM * Name    : BUILD.BAT
REM *
REM * Purpose : Batch file to rebuild SentinelSuperPro Fortran
REM *           Interface
REM *
REM ************************************************************************
REM Save old env setting 

set OLDPATH=%PATH%
REM SET PATH for MSVC

Set path = %path%;"D:\Program Files\Microsoft Visual Studio\VC98";"D:\Program Files\Microsoft Visual Studio\VC98\bin";

REM call VCVARS32.bat file to set MSVC env

call VCVARS32 X86

REM call NMAKE to build applicationdel 
del *.obj /s
del *.exe /s

set NO_EXTERNAL_DEPS=1
 
NMAKE -f SPROEVAL.MAK CFG="sproeval - Win32 MTRelease" 

SET path=%OLDPATH%

REM END OF BATCH file
