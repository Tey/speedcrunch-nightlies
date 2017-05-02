#!/bin/sh
set -e

BASE_DIR=~
SPEEDCRUNCH_SOURCE_DIR=/speedcrunch-source
RELEASE_VERSION=master
OUTPUT_DIR=/vagrant/dist

if [ "$RELEASE_VERSION" = "master" ]; then
    PACKAGE_VERSION=0+$RELEASE_VERSION+snapshot.$(date +"%Y.%m.%d")
else
    PACKAGE_VERSION=$RELEASE_VERSION
fi

function build_speedcrunch {
    ARCH=$*
    echo "Building for '$ARCH'..."
    BUILD_DIR=$BASE_DIR/speedcrunch$ARCH-build
    QT_INSTALL_DIR=$BASE_DIR/qt$ARCH-install
    mkdir -p $BUILD_DIR
    cd $BUILD_DIR
    scl enable devtoolset-2 "bash <<END
    $QT_INSTALL_DIR/bin/qmake $SPEEDCRUNCH_SOURCE_DIR/src CONFIG-=debug QTPLUGIN+='composeplatforminputcontextplugin ibusplatforminputcontextplugin'
    make -j$(nproc --all)
END"
}

function file_expand_vars {
    IN=$1
    OUT=$2
    sed -e "s/@VERSION@/$VARS_VERSION/g;s/@ARCH@/$VARS_ARCH/g;s/@NBYTES@/$VARS_NBYTES/g" $IN > $OUT
    dos2unix -q $OUT
}

function create_icons {
    ICONS_DIR=$*
    for s in 16 22 24 32 48 64 128 256; do
        d=$ICONS_DIR/hicolor/${s}x${s}
        mkdir --mode=0755 -p $d
        mkdir --mode=0755 -p $d/apps
        #rsvg-convert -w $s -h $s $SPEEDCRUNCH_SOURCE_DIR/gfx/speedcrunch.svg -o $d/speedcrunch.png
        convert -resize ${s}x${s} $SPEEDCRUNCH_SOURCE_DIR/src/resources/speedcrunch.png $d/apps/speedcrunch.png
        chmod 0644 $d/apps/speedcrunch.png
    done
}

function create_pkgroot {
    ARCH=$*
    PKGROOT=$BASE_DIR/speedcrunch$ARCH-pkgroot
    echo "Creating package structure for '$ARCH'..."
    # TODO: Use regular installation steps.
    rm -rf $PKGROOT
    mkdir --mode=0755 -p $PKGROOT/opt/speedcrunch
    install --mode 0755 $BASE_DIR/speedcrunch$ARCH-build/speedcrunch $PKGROOT/opt/speedcrunch/
    strip $PKGROOT/opt/speedcrunch/speedcrunch
    install --mode=0644 $SPEEDCRUNCH_SOURCE_DIR/README.md $PKGROOT/opt/speedcrunch/
    dos2unix -q $PKGROOT/opt/speedcrunch/README.md

    mkdir --mode=0755 -p $PKGROOT/usr/bin
    ln -sf /opt/speedcrunch/speedcrunch $PKGROOT/usr/bin/speedcrunch
    create_icons $PKGROOT/usr/share/icons
    mkdir --mode=0755 -p $PKGROOT/usr/share/applications
    install --mode=0644 $SPEEDCRUNCH_SOURCE_DIR/pkg/speedcrunch.desktop $PKGROOT/usr/share/applications/
    dos2unix -q $PKGROOT/usr/share/applications/speedcrunch.desktop
    mkdir --mode=0755 -p $PKGROOT/usr/share/appdata
    install --mode=0644 $SPEEDCRUNCH_SOURCE_DIR/pkg/speedcrunch.appdata.xml $PKGROOT/usr/share/appdata/
    dos2unix -q $PKGROOT/usr/share/appdata/speedcrunch.appdata.xml
}

function build_deb_package {
    ARCH=$*
    PKGROOT=$BASE_DIR/speedcrunch$ARCH-pkgroot
    echo "Building Debian package for '$ARCH'..."
    VARS_VERSION=$PACKAGE_VERSION
    VARS_NBYTES=`du $PKGROOT -s | sed -e "s/\s.*//"`
    if [ $ARCH = 32 ]; then
        VARS_ARCH=i386
    elif [ $ARCH = 64 ]; then
        VARS_ARCH=amd64
    fi
    mkdir -p $PKGROOT/DEBIAN
    file_expand_vars $SPEEDCRUNCH_SOURCE_DIR/pkg/cross-linux/deb-control $PKGROOT/DEBIAN/control
    fakeroot dpkg-deb -b $PKGROOT $OUTPUT_DIR/
    rm -rf $PKGROOT/DEBIAN
}

function build_rpm_package {
    ARCH=$*
    PKGROOT=$BASE_DIR/speedcrunch$ARCH-pkgroot
    echo "Building RPM package for '$ARCH'..."
    VARS_VERSION=$PACKAGE_VERSION
    if [ $ARCH = 32 ]; then
        ARCHNAME=i686
    elif [ $ARCH = 64 ]; then
        ARCHNAME=x86_64
    fi
    SPEC=`mktemp`
    file_expand_vars $SPEEDCRUNCH_SOURCE_DIR/pkg/cross-linux/rpm-specfile $SPEC
    rm -f ~/rpmbuild/RPMS/$ARCHNAME/*.rpm
    rpmbuild -bb $SPEC --buildroot $PKGROOT --target $ARCHNAME
    mv ~/rpmbuild/RPMS/$ARCHNAME/*.rpm $OUTPUT_DIR/
    rm $SPEC
}

function build_tarball {
    ARCH=$*
    PKGROOT=$BASE_DIR/speedcrunch$ARCH-pkgroot
    echo "Building tarball for '$ARCH'..."
    if [ $ARCH = 32 ]; then
        ARCHNAME=i386
    elif [ $ARCH = 64 ]; then
        ARCHNAME=amd64
    fi
    pushd $PKGROOT/opt/speedcrunch
    tar cjf $OUTPUT_DIR/speedcrunch_${PACKAGE_VERSION}_${ARCHNAME}.tar.bz2 *
    popd
}

mkdir -p $OUTPUT_DIR

build_speedcrunch 32
create_pkgroot 32
build_deb_package 32
build_rpm_package 32
build_tarball 32

build_speedcrunch 64
create_pkgroot 64
build_deb_package 64
build_rpm_package 64
build_tarball 64
