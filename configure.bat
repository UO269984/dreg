mklink test\driving-engine.dll ..\engine\build\driving-engine.dll
mklink /D engine\android\jni\src ..\..\src
mklink /D engine\android\jni\include ..\..\include

mklink /D configEditor\wasmEngineApi\include ..\..\engine\include
mklink configEditor\wasmEngineApi\driving-engine.bc ..\..\engine\build\driving-engine.bc

mkdir engine\build
mkdir engine\bin
mkdir test\bin
mkdir configEditor\web\js\driving-engine

make --no-print-directory -C tools