DEPENDENCES_FOLDER=buildDependences

EMSCRIPTEN_VER=1.38.45
EMSDK_VER=3.1.31
EMSDK_ZIP=$EMSDK_VER.zip
EMSDK_FOLDER=emsdk-$EMSDK_VER

NDK_VER=21.4.7075529
SDK_ZIP=commandlinetools-linux-9477386_latest.zip
SDK_FOLDER=SDK

mkdir $DEPENDENCES_FOLDER
cd $DEPENDENCES_FOLDER

#gcc and GNU Make
echo "----- gcc -----"
sudo apt install -y build-essential

#mingw 64
echo -e "\n\n----- MinGW -----"
sudo apt install -y gcc-mingw-w64-x86-64 g++-mingw-w64-x86-64

#Emescriptem 1.38.45
echo -e "\n\n----- Emescriptem 1.38.45 -----"
wget https://github.com/emscripten-core/emsdk/archive/refs/tags/$EMSDK_ZIP
unzip -q $EMSDK_ZIP

rm $EMSDK_ZIP

cd $EMSDK_FOLDER
./emsdk install $EMSCRIPTEN_VER
./emsdk activate $EMSCRIPTEN_VER
cd ..

#Android NDK 21.4.7075529
echo -e "\n\n----- NDK 21.4 -----"
wget https://dl.google.com/android/repository/$SDK_ZIP
unzip -q $SDK_ZIP

rm $SDK_ZIP

mkdir $SDK_FOLDER
echo y | cmdline-tools/bin/sdkmanager --sdk_root=$PWD/$SDK_FOLDER "ndk;$NDK_VER"

echo -e "source $DEPENDENCES_FOLDER/$EMSDK_FOLDER/emsdk_env.sh\n\n\
export PATH=\$PWD/$DEPENDENCES_FOLDER/$SDK_FOLDER/ndk/$NDK_VER:\$PATH\n\
export ANDROID_SDK_ROOT=\$PWD/$DEPENDENCES_FOLDER/$SDK_FOLDER\n" > activate.sh

chmod +x activate.sh

cd ..