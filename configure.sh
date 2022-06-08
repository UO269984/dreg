ln -s ../engine/build/driving-engine.so test/
ln -s ../../src/ engine/android/jni
ln -s ../../include/ engine/android/jni

ln -s ../../engine/include/ configEditor/wasmEngineApi
ln -s ../../engine/build/driving-engine.bc configEditor/wasmEngineApi

mkdir engine/build
mkdir engine/bin
mkdir test/bin

make --no-print-directory -C tools