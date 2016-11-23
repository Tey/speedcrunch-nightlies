VERSION=${1:-master}
sed -i '/set(speedcrunch_VERSION/c\set(speedcrunch_VERSION "'"$VERSION"'")' ../src/CMakeLists.txt
sed -i '/DEFINES += SPEEDCRUNCH_VERSION/c\DEFINES += SPEEDCRUNCH_VERSION=\\\\\\"'"$VERSION"'\\\\\\"' ../src/speedcrunch.pro
sed -i '/RELEASE_VERSION=/c\RELEASE_VERSION='"$VERSION" ../pkg/cross-linux/build-speedcrunch.sh
sed -i '/version =/c\version = '"'$VERSION'" ../doc/src/conf.py
sed -i '/release =/c\release = '"'$VERSION'" ../doc/src/conf.py

