find_package(Qt5Core)


# Basic stuff; locate the Qt5Core DLL so we can determine the other directories from it.
# TODO: This might need to be configurable.
get_target_property(qtwin32_QTCORE_DLL Qt5::Core LOCATION)
get_filename_component(qtwin32_QT_LIBDIR ${qtwin32_QTCORE_DLL} DIRECTORY)
get_filename_component(qtwin32_QT_PLUGINDIR ${qtwin32_QT_LIBDIR}/../plugins REALPATH)
get_filename_component(qtwin32_QT_QMLDIR ${qtwin32_QT_LIBDIR}/../qml REALPATH)
set(qtwin32_DLL_SUFFIX_RELEASE ".dll")
set(qtwin32_DLL_SUFFIX_DEBUG "d.dll")
set(qtwin32_QMLPLUGIN_SUFFIX_RELEASE "plugin${qtwin32_DLL_SUFFIX_RELEASE}")
set(qtwin32_QMLPLUGIN_SUFFIX_DEBUG "plugin${qtwin32_DLL_SUFFIX_DEBUG}")


# Determine the Qt minor version (so '5.4' or '5.5') for version checking. There's unlikely to be enough changes in
# patch releases to make it worth distinguishing based on those.
string(REGEX MATCH "[0-9]+[.][0-9]+" qtwin32_QT_MINOR_VERSION ${Qt5Core_VERSION_STRING})


# qtwin32_check_qt_version([STRICT] <version>...)
# Check the current Qt version and warn if it's not any of the specified versions. Only the first two components of the
# version number are considered (so '5.4' or '5.5'). If STRICT is specified, the build will abort if the versions don't
# match.
function(qtwin32_check_qt_version arg)
  if(${arg} STREQUAL "STRICT" AND ${ARGC} EQUAL 1)
    message(FATAL_ERROR "qtwin32: Not enough arguments")
  endif()

  if(${arg} STREQUAL "STRICT")
    set(mode "SEND_ERROR")
    set(versions ${ARGN})
  else()
    set(mode "WARNING")
    set(versions ${arg} ${ARGN})
  endif()

  foreach(ver ${versions})
    if(${ver} VERSION_EQUAL ${qtwin32_QT_MINOR_VERSION})
      return()
    endif()
  endforeach()

  message(${mode} "qtwin32: Expected Qt versions: ${versions}, actual Qt version: ${qtwin32_QT_MINOR_VERSION}")
endfunction()


# _qtwin32_internal_deploy_dlls(<targetdir> <basedir> <dll name>...)
# Internal function; install the specified libraries from <basedir> to <targetdir>. This function uses the 'install'
# command, so <targetdir> may either be an absolute path or a path relative to CMAKE_INSTALL_PREFIX. The specified names
# may include extra sub directory elements, but are all considered relative to <basedir>. The library names should not
# include the '.dll' suffix or the 'd' infix for debug libraries; if such a debug version of a library is found, it will
# be used for debug configurations.
function(_qtwin32_internal_deploy_dlls targetdir basedir)
  foreach(dll ${ARGN})
    get_filename_component(dlldir ${dll} DIRECTORY)
    set(dbg ${basedir}/${dll}${qtwin32_DLL_SUFFIX_DEBUG})
    set(rel ${basedir}/${dll}${qtwin32_DLL_SUFFIX_RELEASE})
    if(EXISTS ${dbg} AND EXISTS ${rel})
      install(FILES ${rel} DESTINATION ${targetdir}/${dlldir} CONFIGURATIONS Release)
      install(FILES ${dbg} DESTINATION ${targetdir}/${dlldir} CONFIGURATIONS Debug)
    else()
      install(FILES ${rel} DESTINATION ${targetdir}/${dlldir})
    #else()
    #  if(${strict})
    #    message(SEND_ERROR "qtwin32: No matching DLL found for '${dll}', aborting.")
    #  else()
    #    message(WARNING "qtwin32: No matching DLL found for '${dll}', skipping.")
    #  endif()
    endif()
  endforeach()
endfunction()


# qtwin32_deploy_dlls(<targetdir> <dll name>...)
# Install the specified DLLs into <targetdir>. See _qtwin32_internal_deploy_dlls.
function(qtwin32_deploy_dlls targetdir)
  _qtwin32_internal_deploy_dlls(${targetdir} ${qtwin32_QT_LIBDIR} ${ARGN})
endfunction()


# qtwin32_deploy_plugins(<targetdir> <plugin name>...)
# Install the specified plugins into <targetdir>. The plugin name should include the category and the name separated by
# a forward slash, but no file extensions, e.g. 'platforms/qwindows'. See _qtwin32_internal_deploy_dlls.
function(qtwin32_deploy_plugins targetdir)
  _qtwin32_internal_deploy_dlls(${targetdir} ${qtwin32_QT_PLUGINDIR} ${ARGN})
endfunction()


# qtwin32_deploy_qml(<targetdir> <module>...)
# Install QML modules into <targetdir>. This function uses the 'install' command, so <targetdir> may either be an
# absolute path or a path relative to CMAKE_INSTALL_PREFIX. The module names are specified as in QML code, with the name
# components separated by '.'. The major version of the module may be specified at the end of the name, after a ':'
# separator. If the major version is 1, it may be omitted. Examples (QML import statement - module specifier):
#  "import QtQuick 2.0" - QtQuick:2
#  "import QtQuick 2.4" - QtQuick:2
#  "import QtQuick.Window 2.0" - QtQuick.Window:2
#  "import QtQuick.Controls 1.2" - QtQuick.Controls
#  "import QtQuick.Controls 1.2" - QtQuick.Controls:1
function(qtwin32_deploy_qml targetdir)
  foreach(module ${ARGN})
    string(REGEX MATCH "^[^:]+" modulename ${module})
    if(${module} MATCHES ":([0-9]+)")
      set(modulever ${CMAKE_MATCH_1})
    else()
      set(modulever 1)
    endif()
    string(REPLACE "." "/" modulepath ${modulename})
    if(NOT ${modulever} EQUAL 1)
      set(modulepath ${modulepath}.${modulever})
    endif()

    install(DIRECTORY ${qtwin32_QT_QMLDIR}/${modulepath}/
            DESTINATION ${targetdir}/${modulepath}/
            CONFIGURATIONS Release
            PATTERN "*${qtwin32_QMLPLUGIN_SUFFIX_DEBUG}" EXCLUDE)
    install(DIRECTORY ${qtwin32_QT_QMLDIR}/${modulepath}/
            DESTINATION ${targetdir}/${modulepath}/
            CONFIGURATIONS Debug
            PATTERN "*${qtwin32_QMLPLUGIN_SUFFIX_RELEASE}" EXCLUDE)
  endforeach()
endfunction()


# qtwin32_deploy_default_qt_conf(<targetdir>)
# Create and install a basic generated qt.conf into <targetdir>. The generated qt.conf simply sets the "Prefix" field
# to force the Qt libraries to load plugins, QML modules etc. only from the installation directory. This makes sure the
# application doesn't pick up these things from an installed Qt SDK.
function(qtwin32_deploy_default_qt_conf targetdir)
  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/generated-qt.conf "[Paths]\nPrefix=.")
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/generated-qt.conf DESTINATION ${targetdir} RENAME qt.conf)
endfunction()


# _qtwin32_internal_collect_misc_libs()
# Internal function; collect all d3dcompiler_*.dll and ICU DLLs in the Qt installation and save them in
# qtwin32_D3DCompiler_LIBS and qtwin32_ICU_LIBS variables so they can be deployed without hardcoding the specific
# versions. In a function to not pollute the global namespace as much.
function(_qtwin32_internal_collect_misc_libs)
  file(GLOB d3dcompiler_files LIST_DIRECTORIES false "${qtwin32_QT_LIBDIR}/d3dcompiler_*${qtwin32_DLL_SUFFIX_RELEASE}")
  foreach(dll ${d3dcompiler_files})
    get_filename_component(basename ${dll} NAME_WE)
    list(APPEND d3dcompiler_libs ${basename})
  endforeach()
  set(qtwin32_D3DCompiler_LIBS ${d3dcompiler_libs} PARENT_SCOPE)

  file(GLOB icu_files LIST_DIRECTORIES false
       "${qtwin32_QT_LIBDIR}/icudt*${qtwin32_DLL_SUFFIX_RELEASE}"
       "${qtwin32_QT_LIBDIR}/icuin*${qtwin32_DLL_SUFFIX_RELEASE}"
       "${qtwin32_QT_LIBDIR}/icuuc*${qtwin32_DLL_SUFFIX_RELEASE}")
  foreach(dll ${icu_files})
    get_filename_component(basename ${dll} NAME_WE)
    list(APPEND icu_libs ${basename})
  endforeach()
  set(qtwin32_ICU_LIBS ${icu_libs} PARENT_SCOPE)
endfunction()
_qtwin32_internal_collect_misc_libs()


# qtwin32_deploy_modules(<targetdir> <module>...)
# Install modules of files into <targetdir>. A "module", in this context, is a set of Qt DLLs, plugins and QML modules;
# it can also have dependencies on other such modules which will also be installed when the module is installed. For a
# module <name>, the dependent bits are installed as such:
#  any DLLs specified in the variable <name>_LIBS are installed using qtwin32_deploy_dlls
#  any plugins specified in the variable <name>_PLUGINS are installed using qtwin32_deploy_plugins
#  any QML modules specified in the variable <name>_QML are installed using qtwin32_deploy_qml
#  any other modules specified in the variable <name>_DEPS are recursively installed using qtwin32_deploy_modules
function(qtwin32_deploy_modules targetdir)
  foreach(module ${ARGN})
    qtwin32_deploy_modules(${targetdir} ${${module}_DEPS})
    qtwin32_deploy_dlls(${targetdir} ${${module}_LIBS})
    qtwin32_deploy_plugins(${targetdir} ${${module}_PLUGINS})
    qtwin32_deploy_qml(${targetdir} ${${module}_QML})
  endforeach()
endfunction()


# Modules:
# qtwin32_ICU - the ICU libraries
# qtwin32_D3DCompiler - the d3dcompiler_*.dll
# qtwin32_MinGW - MinGW runtime libraries
set(qtwin32_MinGW_LIBS libgcc_s_dw2-1 libstdc++-6 libwinpthread-1)
# qtwin32_Qt5Core - just QtCore
set(qtwin32_Qt5Core_LIBS Qt5Core)
if(MINGW)
  set(qtwin32_Qt5Core_DEPS qtwin32_MinGW)
endif()
# qtwin32_Qt5Network - QtNetwork, without any plugins it might need though
set(qtwin32_Qt5Network_LIBS Qt5Network)
set(qtwin32_Qt5Network_DEPS qtwin32_Qt5Core)
# qtwin32_Qt5Gui - QtGui (without QtQuick or Widgets), including the platform plugin
set(qtwin32_Qt5Gui_LIBS Qt5Gui)
set(qtwin32_Qt5Gui_PLUGINS platforms/qwindows)
set(qtwin32_Qt5Gui_DEPS qtwin32_Qt5Core)
# qtwin32_Qt5Widgets - libraries and plugins for a basic Widgets application
set(qtwin32_Qt5Widgets_LIBS Qt5Widgets)
set(qtwin32_Qt5Widgets_DEPS qtwin32_Qt5Gui)
# qtwin32_Qt5Sql - basic QtSql libraries
set(qtwin32_Qt5Sql_LIBS Qt5Sql)
set(qtwin32_Qt5Sql_DEPS qtwin32_Qt5Core)
# qtwin32_Qt5Help - libraries and plugins for QtHelp
set(qtwin32_Qt5Help_LIBS Qt5Help Qt5CLucene)
set(qtwin32_Qt5Help_PLUGINS sqldrivers/qsqlite)
set(qtwin32_Qt5Help_DEPS qtwin32_Qt5Core qtwin32_Qt5Network qtwin32_Qt5Sql qtwin32_Qt5Widgets)
# qtwin32_Qt5Svg - libraries and plugins for QtSvg (does this really depend on Widgets?)
set(qtwin32_Qt5Svg_LIBS Qt5Svg)
set(qtwin32_Qt5Svg_PLUGINS iconengines/qsvgicon imageformats/qsvg)
set(qtwin32_Qt5Svg_DEPS qtwin32_Qt5Widgets)
# qtwin32_Qt5Quick - libraries and plugins for a basic QtQuick 2 application
set(qtwin32_Qt5Quick_LIBS Qt5Qml Qt5Quick)
set(qtwin32_Qt5Quick_QML QtQuick:2 QtQuick.Window:2)
set(qtwin32_Qt5Quick_DEPS qtwin32_Qt5Network qtwin32_Qt5Gui)
# qtwin32_Qt5WinExtras - QtWinExtras and dependencies
set(qtwin32_Qt5WinExtras_LIBS Qt5WinExtras)
set(qtwin32_Qt5WinExtras_DEPS qtwin32_Qt5Widgets)
# qtwin32_ANGLE - libraries required for ANGLE
set(qtwin32_ANGLE_LIBS libEGL libGLESv2)
if(MSVC)
  # This DLL doesn't seem to be included for the MinGW builds.
  set(qtwin32_ANGLE_DEPS qtwin32_D3DCompiler)
endif()
