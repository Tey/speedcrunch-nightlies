project(speedcrunch)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

if (MSVC)
    add_definitions(-D_USE_MATH_DEFINES)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS)
else()
    add_compile_options(-Wall
                        -Wno-shift-negative-value)
    set(CMAKE_CXX_FLAGS "-std=c++0x")
endif()

if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

set(speedcrunch_VERSION "master")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_MACOSX_RPATH ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Help REQUIRED)

ADD_DEFINITIONS("-DSPEEDCRUNCH_VERSION=\"${speedcrunch_VERSION}\"")
ADD_DEFINITIONS(-DQT_USE_QSTRINGBUILDER)

include(SourceFiles.cmake)

if(APPLE)
    set(APP_ICON_FILE speedcrunch.icns)
    set(APP_ICON "${CMAKE_CURRENT_SOURCE_DIR}/resources/${APP_ICON_FILE}")
    set(APPLICATION_NAME SpeedCrunch)
    set(MACOSX_BUNDLE_ICON_FILE ${APP_ICON_FILE})
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${speedcrunch_VERSION})
    set(MACOSX_BUNDLE_VERSION ${speedcrunch_VERSION})
    set(MACOSX_BUNDLE_LONG_VERSION_STRING Version ${speedcrunch_VERSION})
    set_source_files_properties(${APP_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
else()
    set(APPLICATION_NAME speedcrunch)
endif()

if(WIN32)
    set(WIN32_RES_FILE ${CMAKE_CURRENT_BINARY_DIR}/speedcrunch.rc.obj)
    if(MINGW)
        add_custom_command(OUTPUT ${WIN32_RES_FILE}
                           COMMAND windres.exe ${CMAKE_CURRENT_SOURCE_DIR}/resources/speedcrunch.rc ${WIN32_RES_FILE})
    else()
        add_custom_command(OUTPUT ${WIN32_RES_FILE}
                           COMMAND rc.exe /fo ${WIN32_RES_FILE} ${CMAKE_CURRENT_SOURCE_DIR}/resources/speedcrunch.rc)
    endif()
endif()

# Embedded manual
option(REBUILD_MANUAL "Rebuild the manual rather than using the bundled copy" FALSE)
add_subdirectory(../doc/src doc)
if(REBUILD_MANUAL)
    # set here so CMake knows about this file in this CMakeLists
    set_property(SOURCE ${CMAKE_CURRENT_BINARY_DIR}/doc/manual.qrc PROPERTY GENERATED true)
    set(speedcrunch_RESOURCES ${speedcrunch_RESOURCES} ${CMAKE_CURRENT_BINARY_DIR}/doc/manual.qrc)
else()
    set(speedcrunch_RESOURCES ${speedcrunch_RESOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/../doc/build_html_embedded/manual.qrc)
endif()

if(WIN32)
    add_executable(${APPLICATION_NAME} WIN32 ${speedcrunch_SOURCES} ${speedcrunch_RESOURCES} ${WIN32_RES_FILE})
elseif(APPLE)
    add_executable(${APPLICATION_NAME} MACOSX_BUNDLE ${speedcrunch_SOURCES} ${speedcrunch_RESOURCES} ${APP_ICON})
else()
    add_executable(${APPLICATION_NAME} ${speedcrunch_SOURCES} ${speedcrunch_RESOURCES})
endif()

if(REBUILD_MANUAL)
    set_property(TARGET ${APPLICATION_NAME} PROPERTY AUTOGEN_TARGET_DEPENDS manual)
endif()

add_custom_target(confclean COMMAND rm -rf Makefile CMakeFiles/ CMakeCache.txt cmake_install.cmake DartTestfile.txt install_manifest.txt)

set(QT_LIBRARIES Qt5::Widgets Qt5::Help)
target_link_libraries(${APPLICATION_NAME} ${QT_LIBRARIES})
qt5_use_modules(${APPLICATION_NAME} Widgets Help Sql)

enable_testing()

add_executable(testhmath ${testhmath_SOURCES})
target_link_libraries(testhmath ${QT_LIBRARIES})
add_test(testhmath testhmath)

add_executable(testevaluator ${testevaluator_SOURCES} ${testevaluator_HEADERS_MOC})
target_link_libraries(testevaluator ${QT_LIBRARIES})
add_test(testevaluator testevaluator)

add_executable(testfloatnum ${testfloatnum_SOURCES})
add_test(testfloatnum testfloatnum)

add_executable(testcmath ${testcmath_SOURCES})
target_link_libraries(testcmath ${QT_LIBRARIES})
add_test(testcmath testcmath)

add_executable(testdmath ${testdmath_SOURCES})
target_link_libraries(testdmath ${QT_LIBRARIES})
add_test(testdmath testdmath)

add_executable(testser ${testser_SOURCES})
target_link_libraries(testser ${QT_LIBRARIES})
add_test(testser testser)

set_property(TARGET ${APPLICATION_NAME} APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>:EVALUATOR_DEBUG>)

include_directories(${CMAKE_BINARY_DIR} thirdparty core gui math)

################################# INSTALL ######################################

if(UNIX AND NOT APPLE)
    set(SHARE_DIR "share")
    set(BIN_INSTALL_DIR "bin")

    set(MENU_DIR "${SHARE_DIR}/applications/")
    set(APP_DATA_DIR "${SHARE_DIR}/appdata/")
    set(ICON_DIR "${SHARE_DIR}/pixmaps/")
elseif(APPLE)
    set(BIN_INSTALL_DIR ".")
    set(DOC_INSTALL_DIR ${APPLICATION_NAME}.app/Contents/Resources)
else()
    set(BIN_INSTALL_DIR ".")
    set(DOC_INSTALL_DIR ".")
endif()

option(PORTABLE_SPEEDCRUNCH "Enable to build in portable mode" OFF)
if(PORTABLE_SPEEDCRUNCH)
    add_definitions(-DSPEEDCRUNCH_PORTABLE)
endif(PORTABLE_SPEEDCRUNCH)

install(TARGETS ${APPLICATION_NAME} DESTINATION ${BIN_INSTALL_DIR})

if(UNIX AND NOT APPLE)
    install(FILES ../pkg/speedcrunch.desktop DESTINATION ${MENU_DIR})
    install(FILES resources/speedcrunch.png DESTINATION ${ICON_DIR})
    install(FILES ../pkg/speedcrunch.appdata.xml DESTINATION ${APP_DATA_DIR})
elseif(WIN32)
    install(FILES ../pkg/COPYING.rtf DESTINATION ${DOC_INSTALL_DIR})
endif()

if(WIN32)
    include(QtWin32Deploy)
    qtwin32_deploy_modules(${BIN_INSTALL_DIR} qtwin32_Qt5Widgets qtwin32_Qt5Help)
    qtwin32_deploy_default_qt_conf(${BIN_INSTALL_DIR})
    if(${qtwin32_QT_MINOR_VERSION} VERSION_LESS 5.5)
        # Qt 5.4 and below also require ICU.
        qtwin32_deploy_modules(${BIN_INSTALL_DIR} qtwin32_ICU)
    endif()
elseif(APPLE)
    include(MacdeployQt)
    macdeployqt(${APPLICATION_NAME}.app ${APPLICATION_NAME})
endif(WIN32)

set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION ${BIN_INSTALL_DIR})
include(InstallRequiredSystemLibraries)
set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE)

include(SpeedCrunchCPack.cmake)

################################ UNINSTALL #####################################

configure_file( "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
                "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
                IMMEDIATE @ONLY)

add_custom_target(uninstall "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

