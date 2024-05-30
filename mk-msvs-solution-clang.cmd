@echo off

rem Var section -------------------------------------------------------------------------

rem set BUILD_CONF=Release
set BUILD_CONF=Debug

call %CD%\mk-vars.cmd
call %CD%\mk-trace.cmd

rem Compiler section --------------------------------------------------------------------
set CLIPP_CMAKE_GENERATOR=%MSVC_CMAKE_GENERATOR%
set CLIPP_C_COMPILER=%CLANG_C_COMPILER%
set CLIPP_CXX_COMPILER=%CLANG_CXX_COMPILER%

"%CLIPP_CXX_COMPILER%" --version

rem -------------------------------------------------------------------------------------
pushd %SOURCE_DIR%
cd ..
mkdir %BUILD_DIR% 2>NUL
cd %BUILD_DIR%

rem -------------------------------------------------------------------------------------
rem -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake" ^
rem -DBLD_COMPUTER_NAME="%COMPUTER_NAME%" ^
chcp.com 65001 >NUL
cmake -G %CLIPP_CMAKE_GENERATOR% -T ClangCL,host=x64 -A x64 ^
-DCMAKE_BUILD_TYPE:STRING="%BUILD_CONF%" ^
-DCMAKE_CONFIGURATION_TYPES:STRING="%BUILD_CONF%" ^
-DCMAKE_C_COMPILER:FILEPATH="%CLIPP_C_COMPILER%" ^
-DCMAKE_CXX_COMPILER:FILEPATH="%CLIPP_CXX_COMPILER%" ^
-DCMAKE_RC_COMPILER:FILEPATH="%MSVC_RC_COMPILER%" ^
-DCMAKE_INSTALL_PREFIX="INSTALLED" ^
-DBUILD_TESTING=1 ^
-DBUILD_EXAMPLES=1 ^
%SOURCE_DIR%

IF '%ERRORLEVEL%'=='0' GOTO CMAKE_OK
set RV=%ERRORLEVEL%
echo ERRORLEVEL: %RV%
pause
popd
exit /B %RV%

rem -------------------------------------------------------------------------------------
:CMAKE_OK
start "clipp" clipp.sln
popd
