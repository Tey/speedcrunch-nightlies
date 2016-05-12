#!/bin/sh

SRC_DIR=$(dirname $0)

case "$1" in
extract-strings)
    intltool-extract --local --type gettext/xml $SRC_DIR/speedcrunch.appdata.xml.in
    intltool-extract --local --type gettext/ini $SRC_DIR/speedcrunch.desktop.in
    xgettext tmp/*.h -o desktop-appdata.pot -cTRANSLATORS -a
    ;;
update-metadata-files)
    intltool-merge --xml-style $SRC_DIR $SRC_DIR/speedcrunch.appdata.xml.in $SRC_DIR/../speedcrunch.appdata.xml
    intltool-merge --desktop-style $SRC_DIR $SRC_DIR/speedcrunch.desktop.in $SRC_DIR/../speedcrunch.desktop
    ;;
*)
    echo Commands:
    echo "    extract-strings - generate .pot file"
    echo "    update-metadata-files - update desktop and appdata files"
    ;;
esac
