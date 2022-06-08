mklink test\driving-engine.dll ..\engine\build\driving-engine.dll
mklink engine\android\jni\src ..\..\src
mklink engine\android\jni\include ..\..\include

mklink configEditor\wasmEngineApi\include\ ..\..\engine\include\
mklink configEditor\wasmEngineApi\driving-engine.bc ..\..\engine\build\driving-engine.bc

mkdir engine\build
mkdir engine\bin
mkdir test\bin

make --no-print-directory -C tools