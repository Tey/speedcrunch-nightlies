# How to compile SpeedCrunch for Windows using mingw32 under Linux

## Installing mingw32
Download the binaries for Linux from the mingw-w64 project (File -> Toolchains targetting Win32 -> Automated Builds):
<http://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Automated%20Builds/>

The latest version to date is `mingw-w32-bin_i686-linux_20130523.tar.bz2`. Extract it into a new directory (the archive does not contain a root folder):

    mkdir mingw-w32
    cd mingw-w32
    tar xjf ../mingw-w32-bin_i686-linux_20130523.tar.bz2

Finally, add the binaries to the `PATH` and export the base path for later use:

    export PATH=$PATH:`pwd`/bin
    export MINGW32_PATH=`pwd`

It is also possible to use the system package manager to install mingw32 (`mingw-w64` for Debian/Ubuntu), but the associated compiler must support the C++11 features (g++ 4.9.0 or higher).

## Compiling Qt
There are Qt binaries for mingw, but I did not managed to make them work (the tools are compiled for Windows, but we need them to work with Linux instead).

Download the source code for the latest Qt 5.5 release:
<http://download.qt.io/official_releases/qt/5.5/5.5.0/single/qt-everywhere-opensource-src-5.5.0.tar.gz>

Extract them, and go to the directory:

    tar xzf ./qt-everywhere-opensource-src-5.5.0.tar.gz
    cd qt-everywhere-opensource-src-5.5.0

Generate the Makefiles (takes some minutes):

    nice ./configure -prefix $PWD/install-release-static -static -confirm-license -release -opensource -no-sql-sqlite -no-openssl -no-audio-backend -nomake examples -nomake tests -no-sse2 -xplatform win32-g++ -device-option CROSS_COMPILE=i686-w64-mingw32- -skip qtactiveqt -skip qtmultimedia -skip qtdoc -skip qtcanvas3d -skip qtactiveqt -skip qtenginio -skip qtlocation -skip qtmultimedia -skip qtserialport -skip qtquick1 -skip qtquickcontrols -skip qtscript -skip qtsensors -skip qtwebkit -skip qtwebsockets -skip qtxmlpatterns

Some of the Qt features not used by SpeedCrunch are disabled in order to speed up the build. Also, `qtactiveqt` fails to compile with mingw32.

Then, build Qt:

    nice make install

This will put all the files into the `install-release-static` sub-folder. The building process is pretty long (at least 30 minutes to more than 1 hour). It could be speed up by using the `-j` option of make, which parallelizes the build process. Just specify the number of threads your CPU have, and it should decrease the build time significantly (e.g.: `nice make -j 4 install` for a 4 threads CPU). **Note however that the build process for Qt5.5 does not seem to work well with parallelization.**

Once Qt has been built, set an environment variable to the path which contains the produced files:

    export QT_PATH_W32=`pwd`/install-release-static

If you want to move or rename the `install-release-static` folder, you will need to create the file `qt.conf` in the `bin` subfolder with the following content:

    [Paths]
    Prefix = ..
    

Some Qt tools are also needed to build the manual. They are not built using the previous method and even though they were, they would be built as Windows executable so they cannot be used from Linux. They need to be installed separately. On Ubuntu:

    sudo apt-get install qttools5-dev-tools qt5-default

## Installing Python and required modules
Install [Python](https://www.python.org/downloads/) 2.7+ or 3.4+ and [pip](https://pip.pypa.io/en/latest/installing/) if not already installed. Then install the required modules:

    pip install --user 'sphinx>=1.3' quark-sphinx-theme 'pygments<=2.1.0'

The `--user` option tells pip to install the modules only for the current user so that root access is not needed. The path where the executables have been built needs to be added to the PATH then:

    export PATH=$PATH:$HOME/.local/bin

## Building SpeedCrunch
Download the source code of SpeedCrunch, and extract it:

    wget -O SpeedCrunch-master.zip https://bitbucket.org/heldercorreia/speedcrunch/get/master.zip
    unzip SpeedCrunch-master.zip

Build the Makefiles:

    cd heldercorreia-speedcrunch-*/src
    $QT_PATH_W32/bin/qmake -spec win32-g++ PYTHON_EXECUTABLE=python QCOLLECTIONGENERATOR_EXECUTABLE=/usr/bin/qcollectiongenerator speedcrunch.pro

For the portable version of SpeedCrunch, the option `"DEFINES+=SPEEDCRUNCH_PORTABLE"` needs to be added:

    $QT_PATH_W32/bin/qmake -spec win32-g++ PYTHON_EXECUTABLE=python QCOLLECTIONGENERATOR_EXECUTABLE=/usr/bin/qcollectiongenerator "DEFINES+=SPEEDCRUNCH_PORTABLE" speedcrunch.pro

**FIXME: change `speedcrunch.pro` so that the default Python executable is set to "python" for `win32-g++` (i.e., mingw32)**
**FIXME: the manual does not show up when compiled that way**

Then build the application:

    make

The executable should be available in the `release` sub-folder as `speedcrunch.exe`.

## Pack the SpeedCrunch executable (optional)
The final size of `speedcrunch.exe` is a bit big (>10MB), probably because of all the static libraries it contains. In order to reduce its size, it is possible to pack it with UPX:

    upx --best speedcrunch.exe

This should reduce the size of the executable by 60%-70%. The executable will be erased with the packed version. Note that the version that comes with the Ubuntu package manager is less efficient than the compiled version available on the [UPX project page](http://sourceforge.net/projects/upx/files/upx/), even though the version number is the same (v3.9.1).
