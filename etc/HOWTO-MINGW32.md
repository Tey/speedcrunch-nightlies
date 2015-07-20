# How to compile SpeedCrunch for Windows using mingw32 under Linux

## Installing mingw32
Download the binaries for Linux from the mingw-w64 project (File -> Toolchains targetting Win32 -> Automated Builds):
<http://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Automated%20Builds/>

The latest version to date is `mingw-w32-bin_i686-linux_20130523.tar.bz2`. Extract it into a new directory (the archive does not contain a root folder):
    mkdir mingw-w32
    cd mingw-w32
    tar xjf ../mingw-w32-bin_i686-linux_20130523.tar.bz2

There is a problem with the `windows.h` header, because it is included as `Windows.h` in SpeedCrunch source code (`src/gui/mainwindow.cpp`), and the header names are case sensitive under Linux. The correct name is `Windows.h` (from the Windows Platform SDK), so we need to create a symbolic link from `Windows.h` to `windows.h`:
    cd i686-w64-mingw32/include/
    ln -s windows.h Windows.h
    cd ../..

Finally, add the binaries to the `PATH` and export the base path for later use:
    export PATH=$PATH:`pwd`/bin
    export MINGW32_PATH=`pwd`

## Compiling Qt
There are Qt binaries for mingw, but I did not managed to make them work (the tools are compiled for Windows, but we need them to work with Linux instead).

Download the source code for the latest Qt 4.8 release:
<http://download.qt.io/official_releases/qt/4.8/4.8.7/qt-everywhere-opensource-src-4.8.7.tar.gz>

Extract them, and go to the directory:
    tar xzf ./qt-everywhere-opensource-src-4.8.7.tar.gz
    cd qt-everywhere-opensource-src-4.8.7.tar.gz

Generate the Makefiles (takes some minutes):
    nice ./configure -prefix $PWD/install-release-static -static -confirm-license -release -opensource -no-multimedia -no-audio-backend -no-phonon -no-phonon-backend -nomake examples -nomake demos -nomake docs -no-sse2 -xplatform win32-g++ -device-option CROSS_COMPILE=i686-w64-mingw32-

Some of the Qt features not used by SpeedCrunch are disabled in order to speed up the build.

Then, build Qt:
    nice make install

This will put all the files into the `install-release-static` sub-folder. The building process is pretty long (at least 30 minutes to more than 1 hour). It can be speed up by using the `-j` option of make, which parallelizes the build process. Just specify the number of threads your CPU have, and it should decrease the build time significantly (e.g.: `nice make -j 4 install` for a 4 threads CPU).

Once Qt has been built, set an environment variable to the path which contains the produced files:
    export QT_PATH_W32=`pwd`/install-release-static

## Building SpeedCrunch
Download the source code of SpeedCrunch, and extract it:
    wget -O SpeedCrunch-master.zip https://github.com/speedcrunch/SpeedCrunch/archive/master.zip
    unzip SpeedCrunch-master.zip

Build the Makefiles:
    cd SpeedCrunch-master/src
    $QT_PATH_W32/bin/qmake -spec win32-g++ "QMAKE_LFLAGS += -static" "QMAKE_CXXFLAGS += -std=c++0x" speedcrunch.pro

The "-static" linker flag is needed to avoid the need to add mingw DLL when shipping SpeedCrunch.
The "-std=c++0x" compiler flag is needed in order to enable the C++ 2011 support. Normally, it is set by `speedcrunch.pro`, but a condition prevents that when compiling for Windows.

Then build the application:
    make

The executable should be available in the `release` sub-folder as `speedcrunch.exe`.

## Pack the SpeedCrunch executable (optional)
The final size of `speedcrunch.exe` is a bit big (>10MB), probably because of all the static libraries it contains. In order to reduce its size, it is possible to pack it with UPX:
    upx -9 speedcrunch.exe

This should reduce the size of the executable by 60%-70%. The executable will be erased with the packed version. Note that the version that comes with the Ubuntu package manager is less efficient than the compiled version available on the [UPX project page](http://sourceforge.net/projects/upx/files/upx/), even though the version number is the same (v3.9.1).
