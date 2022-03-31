ln -s ../engine/build/driving-engine.so test/
ln -s ../../src/ engine/android/jni
ln -s ../../include/ engine/android/jni

make --no-print-directory -C tools