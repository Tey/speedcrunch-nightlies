#!/bin/sh
set -e

QT_TARBALL=qt-everywhere-opensource-src-5.6.2.tar.xz
QT_TARBALL_URL=http://download.qt.io/official_releases/qt/5.6/5.6.2/single/$QT_TARBALL

BASE_DIR=~
QT_SOURCE_DIR=$BASE_DIR/qt-source

if [ ! -d ${QT_SOURCE_DIR} ]; then
    wget -O $BASE_DIR/$QT_TARBALL $QT_TARBALL_URL
    mkdir $QT_SOURCE_DIR
    cd $QT_SOURCE_DIR
    tar xvaf $BASE_DIR/$QT_TARBALL --strip-components=1
fi
