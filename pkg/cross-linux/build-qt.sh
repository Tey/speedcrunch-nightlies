#!/bin/sh
set -e

BASE_DIR=~
QT_SOURCE_DIR=$BASE_DIR/qt-source

COMMON_CONFIGURE_ARGS="-opensource -confirm-license \
    -release -static -no-sse3 -no-opengl -no-qml-debug \
    -qt-xcb -qt-xkbcommon-x11 -qt-pcre -qt-zlib -qt-harfbuzz -qt-libpng \
    -fontconfig -dbus-linked -qt-sql-sqlite \
    -nomake tests -nomake examples \
    -skip qt3d -skip qtcanvas3d -skip qtconnectivity -skip qtdoc \
    -skip qtenginio -skip qtgraphicaleffects -skip qtlocation \
    -skip qtmultimedia -skip qtquickcontrols -skip qtquickcontrols2 \
    -skip qtscript -skip qtsensors -skip qtserialbus -skip qtserialport \
    -skip qtwayland -skip qtwebchannel -skip qtwebsockets -skip qtxmlpatterns \
    -skip qtwebengine -skip qtwebview \
"

CONFIGURE_ARGS_32="-xplatform linux-g++-32 -pkg-config"
CONFIGURE_ARGS_64="-platform linux-g++-64"

PKG_CONFIG_PATH_32="/usr/lib/pkgconfig"
PKG_CONFIG_PATH_64="/usr/lib64/pkgconfig"

function build_qt {
    ARCH=$*
    echo "Building for '$ARCH'..."
    BUILD_DIR=$BASE_DIR/qt$ARCH-build
    INSTALL_DIR=$BASE_DIR/qt$ARCH-install
    mkdir -p $BUILD_DIR
    cd $BUILD_DIR
    scl enable devtoolset-2 "bash <<END
    export PKG_CONFIG_PATH=$(eval echo \$PKG_CONFIG_PATH_$ARCH)
    $QT_SOURCE_DIR/configure $COMMON_CONFIGURE_ARGS $(eval echo \$CONFIGURE_ARGS_$ARCH) -prefix $INSTALL_DIR
    make -j$(nproc --all)
    make install
END"
}

build_qt 32
build_qt 64
