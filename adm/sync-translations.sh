SCRIPTDIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOTDIR="$(dirname "$SCRIPTDIR")"
SOURCEDIR="$ROOTDIR/src"
LOCALEDIR="$SOURCEDIR/resources/locale"
PROJECTFILE="$SOURCEDIR/speedcrunch.pro"
APPDATADIR="$ROOTDIR/pkg/desktop-appdata"

# UI strings.
cd $LOCALEDIR
tx pull -f
cd $SOURCEDIR
lupdate -no-obsolete $PROJECTFILE
lrelease -removeidentical -compress $PROJECTFILE
cd $LOCALEDIR
tx push -s -t

# Linux AppData strings.
tx pull -f
./metadata-tool.sh update-metadata-files
./metadata-tool.sh extract-strings
tx push -s -t

