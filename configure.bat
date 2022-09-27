mkdir test\include

mklink test\dreg-debug.dll ..\engine\build\dreg-debug.dll
mklink /D test\include\dreg ..\..\engine\include
mklink /D test\unit\include\dreg ..\..\..\engine\include
mklink test\unit\dreg-debug.dll ..\..\engine\build\dreg-debug.dll
mklink /D engine\android\jni\src ..\..\src
mklink /D engine\android\jni\include ..\..\include
mklink /D test\performance\include\dreg ..\..\..\engine\include
mklink /D test\performance\dreg.so ..\..\engine\build\dreg.so

mklink /D configEditor\wasmEngineApi\include ..\..\engine\include
mklink configEditor\wasmEngineApi\dreg.bc ..\..\engine\build\dreg.bc

mkdir engine\build
mkdir engine\bin\debug
mkdir engine\bin\release

mkdir test\bin
mkdir test\unit\bin
mkdir test\performance\bin
mkdir test\performance\bin\win
mkdir configEditor\web\js\dreg

make --no-print-directory -C tools