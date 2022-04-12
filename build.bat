@echo off

where /q cl
IF ERRORLEVEL 1 (
  call "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x64
)

>shaders.h (
  echo #ifndef SHADERS_H
  echo #define SHADERS_H
  echo.
)

for %%f in (*.glsl) do (
  >%%f.gen (
    echo R"_SHADER_GEN(
    type %%f
    echo ^)_SHADER_GEN"
  )
  >>shaders.h (
    echo static const char *%%~nf_text = {
    echo #include "%%f.gen"
    echo };
  )
)

>>shaders.h (
  echo.
  echo #endif //SHADERS_H
)

set hour=%time:~0,2%
if "%hour:~0,1%" == " " set hour=0%hour:~1,1%
set min=%time:~3,2%
if "%min:~0,1%" == " " set min=0%min:~1,1%
set secs=%time:~6,2%
if "%secs:~0,1%" == " " set secs=0%secs:~1,1%

set "pdb_filename=renderer_software_%date:~-2%%date:~-10,-8%%date:~-7,-5%_%hour%%min%%secs%.pdb"

if "%1"=="Debug" (
  echo --- Building in Debug mode ---
  mkdir Debug >NUL 2>NUL
  
  pushd Debug
  if not exist imgui_impl_opengl3.obj (
    cl /nologo /Zi /MD /I ..\lib\include /FC /c ..\lib\include\IMGUI\backends\imgui_impl_glfw.cpp ..\lib\include\IMGUI\backends\imgui_impl_opengl3.cpp
  )
  echo Waiting for PDB > pdb_lock.tmp

  del renderer_software_*.pdb
  cl /nologo /Zi /MDd /I ..\lib\include /FC /Fmrenderer_software.map ..\renderer_software.cpp /LD /link /INCREMENTAL:NO /PDB:%pdb_filename%
  del pdb_lock.tmp
  cl /nologo /Zi /MD /I ..\lib\include /FC /DBUILD_DEBUG /Femain.exe ..\main.cpp /link /INCREMENTAL:NO  /LIBPATH:..\lib\glfw\lib-vc2019 glfw3.lib gdi32.lib user32.lib kernel32.lib shell32.lib imgui_impl_opengl3.obj imgui_impl_glfw.obj
  popd
) else (
  echo --- Building in Release mode ---
  mkdir Release >NUL 2>NUL
  
  pushd Release
  if not exist imgui_impl_opengl3.obj (
    cl /nologo /O2 /MD /I ..\lib\include /FC /c ..\lib\include\IMGUI\backends\imgui_impl_glfw.cpp ..\lib\include\IMGUI\backends\imgui_impl_opengl3.cpp
  )
  cl /nologo /O2 /MD /I ..\lib\include /FC /Femain.exe ..\main.cpp /link /LIBPATH:..\lib\glfw\lib-vc2019 glfw3.lib gdi32.lib user32.lib kernel32.lib shell32.lib imgui_impl_opengl3.obj imgui_impl_glfw.obj
  popd
)