ln -s ../engine/build/driving-engine.so test/
ln -s ../../src/ engine/android/jni
ln -s ../../include/ engine/android/jni

mkdir engine/build
mkdir engine/bin
mkdir test/bin

make --no-print-directory -C tools