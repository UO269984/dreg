mklink test\dreg.dll ..\engine\build\dreg.dll
mklink /D test\include\dreg ..\..\engine\include
mklink /D engine\android\jni\src ..\..\src
mklink /D engine\android\jni\include ..\..\include

mklink /D configEditor\wasmEngineApi\include ..\..\engine\include
mklink configEditor\wasmEngineApi\dreg.bc ..\..\engine\build\dreg.bc

mkdir engine\build
mkdir engine\bin
mkdir test\bin
mkdir configEditor\web\js\dreg

make --no-print-directory -C tools