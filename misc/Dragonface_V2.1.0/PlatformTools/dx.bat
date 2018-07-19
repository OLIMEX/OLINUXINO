@echo off
REM Copyright (C) 2007 The Android Open Source Project
REM
REM Licensed under the Apache License, Version 2.0 (the "License");
REM you may not use this file except in compliance with the License.
REM You may obtain a copy of the License at
REM
REM     http://www.apache.org/licenses/LICENSE-2.0
REM
REM Unless required by applicable law or agreed to in writing, software
REM distributed under the License is distributed on an "AS IS" BASIS,
REM WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
REM See the License for the specific language governing permissions and
REM limitations under the License.

REM don't modify the caller's environment
setlocal

REM Locate dx.jar in the directory where dx.bat was found and start it.

REM Set up prog to be the path of this script, including following symlinks,
REM and set up progdir to be the fully-qualified pathname of its directory.
set prog=%~f0

REM Change current directory to where dx is, to avoid issues with directories
REM containing whitespaces.
cd /d %~dp0

set jarfile=dx.jar
set frameworkdir=

if exist %frameworkdir%%jarfile% goto JarFileOk
    set frameworkdir=lib\

if exist %frameworkdir%%jarfile% goto JarFileOk
    set frameworkdir=..\framework\

:JarFileOk

set jarpath=%frameworkdir%%jarfile%

set javaOpts=
set args=

REM By default, give dx a max heap size of 1 gig. This can be overridden
REM by using a "-JXmx..." option (see below).
set defaultMx=-Xmx1024M

REM capture all arguments to process them below
set params=%*

:nextArg
if "%params%"=="" goto endArgs
    REM Note: advanced substitions don't work on %1..%N. We need to assign to
    REM a variable first.
    REM We also can't use %1..%N directly because an option such as --output=name
    REM gets automagically converted into %1=--output and %2=name (yes, really!)
    REM Instead we manually extract the first token from the params variable.
    for /F "tokens=1*" %%a in ("%params%") do call :getArg "%%a" "%%b"

    if "%defaultMx%"=="" goto notXmx
    if "%A:~0,5%" NEQ "-JXmx" goto notXmx
        set defaultMx=
    :notXmx

    if "%A:~0,2%" NEQ "-J" goto notJ
        set javaOpts=%javaOpts% -%A:~2%
        goto nextArg

    :notJ
        set args=%args% %A%
        goto nextArg

:getArg
    REM this subroutine is called by the for /F with the first argument of params
    REM and the rest of the line. The "goto :eof" actually exits the subroutine.
    set A=%~1
    set params=%~2
    goto :eof

:endArgs

set javaOpts=%javaOpts% %defaultMx%

call java %javaOpts% -Djava.ext.dirs=%frameworkdir% -jar %jarpath% %args%
