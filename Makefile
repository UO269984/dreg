
MKDIR := mkdir

.PHONY: configure
configure: folders links
	@$(MAKE) --no-print-directory -C tools

.PHONY: folders
folders:
	$(MKDIR) engine/build
	$(MKDIR) engine/bin
	$(MKDIR) engine/bin/debug
	$(MKDIR) engine/bin/release
	$(MKDIR) engine/bin/win-debug
	$(MKDIR) engine/bin/win-release
	
	$(MKDIR) test/include
	$(MKDIR) test/bin
	$(MKDIR) test/unit/bin
	$(MKDIR) test/performance/bin
	$(MKDIR) test/performance/bin/win
	$(MKDIR) test/performance/wasm/web/js/dreg
	$(MKDIR) configEditor/web/js/dreg

ifeq ($(OS),Windows_NT)
.PHONY: links
links:
	cmd /C "mklink test\dreg-debug.dll ..\engine\build\dreg-debug.dll"
	cmd /C "mklink /D test\include\dreg ..\..\engine\include"
	cmd /C "mklink /D test\unit\include\dreg ..\..\..\engine\include"
	cmd /C "mklink test\unit\dreg-debug.dll ..\..\engine\build\dreg-debug.dll"
	cmd /C "mklink /D engine\android\jni\src ..\..\src"
	cmd /C "mklink /D engine\android\jni\include ..\..\include"
	cmd /C "mklink /D test\performance\include\dreg ..\..\..\engine\include"
	cmd /C "mklink test\performance\dreg.dll ..\..\engine\build\dreg.dll"
	cmd /C "mklink test\performance\wasm\web\dregStyle.css ..\..\..\..\configEditor\web\dregStyle.css"
	cmd /C "mklink /D test\performance\wasm\web\fonts ..\..\..\..\configEditor\web\fonts"
	cmd /C "mklink test\performance\wasm\web\js\AppLoadManager.js ..\..\..\..\..\configEditor\web\js\AppLoadManager.js"
	cmd /C "mklink /D test\performance\android\app\src\main\cpp\include ..\..\..\..\..\include"
	cmd /C "mklink  test\performance\android\app\src\main\cpp\testPerformance.cpp ..\..\..\..\..\src\testPerformance.cpp"
	cmd /C "mklink /D test\performance\android\app\src\main\cpp\dregLibs ..\..\..\..\..\..\..\engine\build\androidLibs"
	
	cmd /C "mklink /D configEditor\wasmEngineApi\include ..\..\engine\include"
	cmd /C "mklink configEditor\wasmEngineApi\dreg.bc ..\..\engine\build\dreg.bc"

else
.PHONY: links
links:
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
	ln -s ../../../../../include test/performance/android/app/src/main/cpp
	ln -s ../../../../../src/testPerformance.cpp test/performance/android/app/src/main/cpp
	ln -s ../../../../../../../engine/build/androidLibs test/performance/android/app/src/main/cpp/dregLibs
	
	ln -s ../../src/ engine/android/jni
	ln -s ../../include/ engine/android/jni
	
	ln -s ../../engine/include/ configEditor/wasmEngineApi
	ln -s ../../engine/build/dreg.bc configEditor/wasmEngineApi
endif

.PHONY: clean-configure
clean-configure:
	@$(MAKE) --no-print-directory -C tools clean
	rm -rf engine/build
	rm -rf engine/bin
	rm -rf engine/bin/debug
	rm -rf engine/bin/release
	rm -rf engine/bin/win-debug
	rm -rf engine/bin/win-release
	rm -rf test/include
	rm -rf test/bin
	rm -rf test/unit/bin
	rm -rf test/performance/bin
	rm -rf test/performance/bin/win
	rm -rf test/performance/wasm/web/js/dreg
	rm -rf configEditor/web/js/dreg
	rm -f test/dreg-debug.so
	rm -f test/dreg-debug.dll
	rm -f test/include/dreg
	rm -f test/unit/include/dreg
	rm -f test/unit/dreg-debug.so
	rm -f test/unit/dreg-debug.dll
	rm -f test/performance/include/dreg
	rm -f test/performance/dreg.so
	rm -f test/performance/dreg.dll
	rm -f test/performance/wasm/web/dregStyle.css
	rm -f test/performance/wasm/web/fonts
	rm -f test/performance/wasm/web/js/AppLoadManager.js
	rm -f test/performance/android/app/src/main/cpp/include
	rm -f test/performance/android/app/src/main/cpp/testPerformance.cpp
	rm -f test/performance/android/app/src/main/cpp/dregLibs
	rm -f engine/android/jni/src
	rm -f engine/android/jni/include
	rm -f configEditor/wasmEngineApi/include
	rm -f configEditor/wasmEngineApi/dreg.bc


.PHONY: test
test:
	cd test; ./test; cd ..
	
	@$(MAKE) -C test/unit test
	@$(MAKE) -C test/performance test

.PHONY: all
all:
	@$(MAKE) --no-print-directory -C engine all
	@$(MAKE) --no-print-directory -C test
	@$(MAKE) --no-print-directory -C test/unit
	@$(MAKE) --no-print-directory -C test/performance all
	cd configEditor/wasmEngineApi/; ./compile.sh; cd ../..

.PHONY: clean
clean:
	@$(MAKE) --no-print-directory -C engine clean
	@$(MAKE) --no-print-directory -C test clean
	@$(MAKE) --no-print-directory -C test/unit clean
	@$(MAKE) --no-print-directory -C test/performance clean
	rm -rf configEditor/web/js/dreg/*