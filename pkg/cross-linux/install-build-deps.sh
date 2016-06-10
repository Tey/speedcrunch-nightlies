#!/bin/sh
set -e

COMMON_DEPS="devtoolset-2 scl-utils \
    glibc-devel.i686 glibc-devel.x86_64 \
    devtoolset-2-libstdc++-devel.i686 devtoolset-2-libstdc++-devel.x86_64 \
    xkeyboard-config
    xz dpkg rpm-build ImageMagick fakeroot dos2unix"

PERARCH_DEPS="fontconfig-devel freetype-devel \
    libX11-devel libXext-devel libXfixes-devel libXi-devel libXrender-devel libxcb-devel \
    dbus-devel"

ALL_DEPS=$COMMON_DEPS
for dep in $PERARCH_DEPS; do
    ALL_DEPS="$ALL_DEPS $dep.x86_64 $dep.i686"
done

sudo wget -O /etc/yum.repos.d/slc6-devtoolset.repo http://linuxsoft.cern.ch/cern/devtoolset/slc6-devtoolset.repo
sudo wget -O /etc/pki/rpm-gpg/RPM-GPG-KEY-cern http://linuxsoft.cern.ch/cern/slc6X/x86_64/RPM-GPG-KEY-cern

sudo yum install -y $ALL_DEPS
