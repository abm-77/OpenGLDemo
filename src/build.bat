@echo off

rem Set up directories
set GLEW_INCLUDE="..\dependencies\GLEW\include"
set GLEW_LIB="..\dependencies\GLEW\lib\glew32.lib"

set GLFW_INCLUDE="..\dependencies\GLFW\include"
set GLFW_LIB="..\dependencies\GLFW\lib\glfw3.lib"

set GLM_INCLUDE="..\dependencies\GLM\include"

call vcvars32

rem If build directory doesn't exist, make it
IF NOT EXIST ..\build mkdir ..\build

rem Go into build directory
pushd ..\build 

rem Delete all obj files
del *.obj

rem compile and link
cl /I \INCLUDE /I %GLFW_INCLUDE% /I %GLEW_INCLUDE% /I %GLM_INCLUDE% -nologo /FC /Zi /MD /EHsc ..\src\*.cpp  user32.lib gdi32.lib shell32.lib %GLFW_LIB% %GLEW_LIB%  opengl32.lib /Fe:application.exe


del *.obj

popd
