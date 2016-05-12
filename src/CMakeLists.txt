cmake_minimum_required(VERSION 2.8.11)

project(speedcrunch)
set(speedcrunch_VERSION "master" )

set(CMAKE_AUTOMOC ON)
set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Help)

if(APPLE)
    set(PROGNAME SpeedCrunch)
    set(MACOSX_BUNDLE_ICON_FILE speedcrunch.icns)
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${speedcrunch_VERSION})
    set(MACOSX_BUNDLE_VERSION ${speedcrunch_VERSION})
    set(MACOSX_BUNDLE_LONG_VERSION_STRING Version ${speedcrunch_VERSION})
    set(CMAKE_OSX_ARCHITECTURES ppc;i386)
else(APPLE)
    set(PROGNAME speedcrunch)
endif(APPLE)

ADD_DEFINITIONS("-DSPEEDCRUNCH_VERSION=\"${speedcrunch_VERSION}\"")
ADD_DEFINITIONS(-DQT_USE_QSTRINGBUILDER)

include(SourceFiles.cmake)

if(WIN32)
    set(WIN32_RES_FILE ${CMAKE_CURRENT_BINARY_DIR}/speedcrunch.rc.obj)
    if(MINGW)
        add_custom_command(OUTPUT ${WIN32_RES_FILE}
                           COMMAND windres.exe ${CMAKE_CURRENT_SOURCE_DIR}/resources/speedcrunch.rc ${WIN32_RES_FILE})
    else(MINGW)
        add_custom_command(OUTPUT ${WIN32_RES_FILE}
                           COMMAND rc.exe /fo ${WIN32_RES_FILE} ${CMAKE_CURRENT_SOURCE_DIR}/resources/speedcrunch.rc)
    endif(MINGW)
endif(WIN32)

string(COMPARE EQUAL "${CMAKE_CXX_COMPILER_ID}" MSVC USING_MSVC)
if(USING_MSVC)
    add_definitions(-D_USE_MATH_DEFINES)
else()
    add_definitions(-Wall)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
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

qt5_add_RESOURCES(speedcrunch_RESOURCES_SOURCES ${speedcrunch_RESOURCES})
add_executable(${PROGNAME} WIN32 MACOSX_BUNDLE ${speedcrunch_SOURCES} ${speedcrunch_HEADERS_MOC} ${speedcrunch_RESOURCES_SOURCES} ${speedcrunch_FORMS_HEADERS} ${WIN32_RES_FILE})

if(REBUILD_MANUAL)
    add_dependencies(${PROGNAME} manual)
endif()

if(APPLE)
    set( speedcrunch_RESOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/${PROGNAME}.app/Contents/Resources )
    add_custom_command(TARGET ${PROGNAME} POST_BUILD
                       COMMAND mkdir ARGS -p ${speedcrunch_RESOURCE_DIR}
                       COMMAND cp ARGS -f resources/${MACOSX_BUNDLE_ICON_FILE} ${speedcrunch_RESOURCE_DIR})
endif(APPLE)

add_custom_target(confclean COMMAND rm -rf Makefile CMakeFiles/ CMakeCache.txt cmake_install.cmake DartTestfile.txt install_manifest.txt)

set(QT_LIBRARIES Qt5::Widgets Qt5::Help)
target_link_libraries(${PROGNAME} ${QT_LIBRARIES})

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

set_property(TARGET ${PROGNAME} APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>:EVALUATOR_DEBUG>)

include_directories(${CMAKE_BINARY_DIR} thirdparty core gui math)

################################# INSTALL ######################################

if(NOT WIN32)
    set(SHAREDIR "share/")
    set(MENUDIR "${SHAREDIR}/applications/")
    set(APPDATADIR "${SHAREDIR}/appdata/")
    set(ICONDIR "${SHAREDIR}/pixmaps/")
    set(BINDIR "bin")
else(NOT WIN32)
    set(BINDIR ".")
    set(DATADIR ".")
endif(NOT WIN32)

option(PORTABLE_SPEEDCRUNCH "Enable to build in portable mode" OFF)
if(PORTABLE_SPEEDCRUNCH)
    add_definitions(-DSPEEDCRUNCH_PORTABLE)
endif(PORTABLE_SPEEDCRUNCH)

install(TARGETS ${PROGNAME} DESTINATION ${BINDIR})

if(NOT WIN32)
    install(FILES ../pkg/speedcrunch.desktop DESTINATION ${MENUDIR})
    install(FILES resources/speedcrunch.png DESTINATION ${ICONDIR})
    install(FILES ../pkg/speedcrunch.appdata.xml DESTINATION ${APPDATADIR})
else(NOT WIN32)
    install(FILES ../pkg/COPYING.rtf DESTINATION ${DATADIR})
endif(NOT WIN32)

if(WIN32)
    include(../pkg/QtWin32Deploy.cmake)
    qtwin32_deploy_modules(${BINDIR} qtwin32_Qt5Widgets qtwin32_Qt5Help)
    qtwin32_deploy_default_qt_conf(${BINDIR})
    if(${qtwin32_QT_MINOR_VERSION} VERSION_LESS 5.5)
        # Qt 5.4 and below also require ICU.
        qtwin32_deploy_modules(${BINDIR} qtwin32_ICU)
    endif()
endif(WIN32)
set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION ${BINDIR})
include(InstallRequiredSystemLibraries)

################################# PACKAGE #####################################

set(CPACK_PACKAGE_VENDOR SpeedCrunch)
set(CPACK_PACKAGE_VERSION ${speedcrunch_VERSION})
set(CPACK_PACKAGE_INSTALL_DIRECTORY SpeedCrunch)
set(CPACK_PACKAGE_EXECUTABLES ${PROGNAME} SpeedCrunch)

if(WIN32)
    if(NOT CPACK_GENERATOR)
        if(PORTABLE_SPEEDCRUNCH)
            set(CPACK_GENERATOR ZIP)
        else(PORTABLE_SPEEDCRUNCH)
            set(CPACK_GENERATOR NSIS)
        endif(PORTABLE_SPEEDCRUNCH)
    endif(NOT CPACK_GENERATOR)

    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/../pkg/COPYING.rtf")
    set(CPACK_NSIS_EXECUTABLES_DIRECTORY ${BINDIR})
    set(CPACK_NSIS_INSTALLED_ICON_NAME "${BINDIR}\\\\${PROGNAME}.exe,0")
    set(CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}\\\\resources\\\\speedcrunch.ico")
    set(CPACK_NSIS_HELP_LINK "http://groups.google.com/group/speedcrunch")
    set(CPACK_NSIS_URL_INFO_ABOUT "http://speedcrunch.org")
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL true)

    set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS
        "WriteRegStr SHCTX 'Software\\\\RegisteredApplications' 'SpeedCrunch' 'Software\\\\SpeedCrunch\\\\Capabilities'
         WriteRegStr SHCTX 'Software\\\\SpeedCrunch\\\\Capabilities' 'ApplicationDescription' 'SpeedCrunch is a high-precision scientific calculator'
         WriteRegStr SHCTX 'Software\\\\SpeedCrunch\\\\Capabilities' 'ApplicationName' 'SpeedCrunch'
         WriteRegStr SHCTX 'Software\\\\SpeedCrunch\\\\Capabilities\\\\UrlAssociations' 'calculator' 'SpeedCrunch.Url.calculator'

         WriteRegStr SHCTX 'Software\\\\Classes\\\\SpeedCrunch.Url.calculator' '' 'SpeedCrunch'
         WriteRegStr SHCTX 'Software\\\\Classes\\\\SpeedCrunch.Url.calculator' 'FriendlyTypeName' 'SpeedCrunch'
         WriteRegStr SHCTX 'Software\\\\Classes\\\\SpeedCrunch.Url.calculator\\\\shell\\\\open\\\\command' '' '\\\"$INSTDIR\\\\${BINDIR}\\\\${PROGNAME}.exe\\\"'
         WriteRegStr SHCTX 'Software\\\\Classes\\\\SpeedCrunch.Url.calculator\\\\Application' 'ApplicationCompany' 'SpeedCrunch'

         WriteRegStr SHCTX 'Software\\\\Classes\\\\Applications\\\\${PROGNAME}.exe' 'FriendlyAppName' 'SpeedCrunch'
         ")
    set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS
        "DeleteRegValue SHCTX 'Software\\\\RegisteredApplications' 'SpeedCrunch'
         DeleteRegKey SHCTX 'Software\\\\SpeedCrunch'
         DeleteRegKey SHCTX 'Software\\\\Classes\\\\SpeedCrunch.Url.calculator'
         DeleteRegKey SHCTX 'Software\\\\Classes\\\\Applications\\\\${PROGNAME}.exe'
         ")

    if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
        # When building a 64-bit installer, we add a check to make it fail on 32-bit systems
        # because CPack doesn't do that by itself. Using this variable is probably a bit of a hack
        # though.
        set(CPACK_NSIS_DEFINES
            "!include 'x64.nsh'
             Function x64Check
             \\\${IfNot} \\\${RunningX64}
                 MessageBox MB_OK|MB_ICONEXCLAMATION 'This installer can only be run on 64-bit Windows.'
                 Quit
             \\\${EndIf}
             FunctionEnd

             Page custom x64Check
            ")
    endif()
endif(WIN32)

include(CPack)

################################ UNINSTALL #####################################

configure_file( "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
                "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
                IMMEDIATE @ONLY)

add_custom_target(uninstall "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

