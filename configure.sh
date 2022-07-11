ln -s ../engine/build/dreg.so test/
ln -s ../../engine/include/ test/include/dreg
ln -s ../../src/ engine/android/jni
ln -s ../../include/ engine/android/jni

ln -s ../../engine/include/ configEditor/wasmEngineApi
ln -s ../../engine/build/dreg.bc configEditor/wasmEngineApi

mkdir engine/build
mkdir engine/bin
mkdir engine/bin-win
mkdir test/bin
mkdir configEditor/web/js/dreg

make --no-print-directory -C tools