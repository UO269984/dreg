mkdir test/include

ln -s ../engine/build/dreg-debug.so test/
ln -s ../../engine/include/ test/include/dreg
ln -s ../../../engine/include/ test/unit/include/dreg
ln -s ../../engine/build/dreg-debug.so test/unit
ln -s ../../../engine/include/ test/performance/include/dreg
ln -s ../../engine/build/dreg.so test/performance

ln -s ../../src/ engine/android/jni
ln -s ../../include/ engine/android/jni

ln -s ../../engine/include/ configEditor/wasmEngineApi
ln -s ../../engine/build/dreg.bc configEditor/wasmEngineApi

mkdir engine/build
mkdir engine/bin
mkdir engine/bin/debug
mkdir engine/bin/release
mkdir engine/bin/win-debug
mkdir engine/bin/win-release

mkdir test/bin
mkdir test/unit/bin
mkdir test/performance/bin
mkdir test/performance/bin/win
mkdir configEditor/web/js/dreg

make --no-print-directory -C tools