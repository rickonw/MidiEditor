set oldpath=%~dp0
mkdir lib
cd lib
mkdir x64
cd x64
mkdir  vc15
cd vc15
cmake -DNO_CONSOLE_MODE=ON -DQT5_DIR=D:\Qt\Qt5.9.1\5.9.1\msvc2017_64\lib\cmake -DINSTALL_WITH_CPACK=ON -G "Visual Studio 15 2017 Win64" ../../../../lib
cmake --build . --config Release
cpack -DCPACK_GENERATOR=WIX -C Release

cd %oldpath%
PAUSE
