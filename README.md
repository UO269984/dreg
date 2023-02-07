![Dreg logo](logos/dreg-logo-landscape.png)

Dreg is a general purpose driving physics engine, in other words, a self contained library capable of simulating the physics of a wheeled vehicle, which can be used from any programming language and in any development environment.

Dreg tries to be a cross-platform library, at the moment it supports running on Windows, Linux, Web (via Wasm) and Android.

It allows configuring the vehicle in a pretty flexible way, allowing to define some parameters as 2D graphs. [This editor](https://uo269984.github.io/TFG/editor/) simplifies the task of creating a configuration.

A set of performance tests is also included to measure the library performance in any platform. The binaries of these can be found in the [latest release](https://github.com/UO269984/dreg/releases/latest), or can be built as shown below. The web version is available to run at [this link](https://uo269984.github.io/TFG/performanceTest/).
**Notice:** Before running the tests on Windows, run `chcp 65001` in the terminal where you are going to run the tests to set the right encoding.


## Build project
The build system is based on a set of Makefiles. It uses GCC to build the Linux version, mingw64 for Windows, Emscripten for WebAssembly and NDK for Android. To build the Linux version Clang has also been tested, showing a bit more performance than GCC. It can be used by adding `CXX=clang` as a parameter in the make command run.

Although no more C++ compilers apart from the mentioned have been tested, others outside of that list may also work, as Dreg does not rely on any compiler specific feature.

To link all subprojects that Dreg is made of, system symbolic links are used. Those links have to be created before compiling or running the project. To create them automatically run `make configure` in the project root folder.

```sh
make configure #Configure symbolic links between subprojects
make all #Build all subprojects
```

After the build, the Dreg libraries for all platforms are placed in the _engine/build_ folder. The build also generates executables to run the unit and performance tests in the folders _test/unit_ and _test/performace_. After the build, the editor is also ready to be hosted as a web in the folder _configEditor/web_.

## Examples
In [this repo](https://github.com/UO269984/dreg-demos) you can find an interactive vehicle demo developed in Unity.

## Name origin
The name **Dreg** comes from mixing letters from _**dr**iving **e**n**g**ine_. This is a short, simple and easy to remember word, but in addition, it's a play on words, since Dreg is a small variation of the _drag_ word, referring to vehicle aerodynamic drag.