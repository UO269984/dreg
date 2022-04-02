mklink test\driving-engine.dll ..\engine\build\driving-engine.dll
mklink engine\android\jni\src ..\..\src
mklink engine\android\jni\include ..\..\include

make --no-print-directory -C tools