mkdir test\include

mklink test\dreg-debug.dll ..\engine\build\dreg-debug.dll
mklink /D test\include\dreg ..\..\engine\include
mklink /D test\unit\include\dreg ..\..\..\engine\include
mklink test\unit\dreg-debug.dll ..\..\engine\build\dreg-debug.dll
mklink /D engine\android\jni\src ..\..\src
mklink /D engine\android\jni\include ..\..\include
mklink /D test\performance\include\dreg ..\..\..\engine\include
mklink test\performance\dreg.dll ..\..\engine\build\dreg.dll
mklink test\performance\wasm\web\dregStyle.css ..\..\..\..\configEditor\web\dregStyle.css
mklink /D test\performance\wasm\web\fonts ..\..\..\..\configEditor\web\fonts\
mklink test\performance\wasm\web\js\AppLoadManager.js ..\..\..\..\..\configEditor\web\js\AppLoadManager.js

mklink /D configEditor\wasmEngineApi\include ..\..\engine\include
mklink configEditor\wasmEngineApi\dreg.bc ..\..\engine\build\dreg.bc

mkdir engine\build
mkdir engine\bin\debug
mkdir engine\bin\release

mkdir test\bin
mkdir test\unit\bin
mkdir test\performance\bin
mkdir test\performance\wasm\web\js\dreg
mkdir configEditor\web\js\dreg

make --no-print-directory -C tools