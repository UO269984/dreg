mkdir test/include

ln -s ../engine/build/dreg-debug.so test/
ln -s ../../engine/include/ test/include/dreg
ln -s ../../../engine/include/ test/unit/include/dreg
ln -s ../../engine/build/dreg-debug.so test/unit
ln -s ../../../engine/include/ test/performance/include/dreg
ln -s ../../engine/build/dreg.so test/performance
ln -s ../../engine/build/dreg.dll test/performance
ln -s ../../../../configEditor/web/dregStyle.css test/performance/wasm/web
ln -s ../../../../configEditor/web/fonts/ test/performance/wasm/web
ln -s ../../../../../configEditor/web/js/AppLoadManager.js test/performance/wasm/web/js

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
mkdir test/performance/wasm/web/js/dreg
mkdir configEditor/web/js/dreg

make --no-print-directory -C tools