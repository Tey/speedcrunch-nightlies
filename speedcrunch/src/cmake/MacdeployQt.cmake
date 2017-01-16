# Macdeployqt.cmake
#
# Copyright (c) 2017 Samuel Gaist <samuel.gaist@edeltech.ch>
#

function(macdeployqt target app_name)

    set(CURRENT_TARGET_PATH "\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${BIN_INSTALL_DIR}/${target}")
    set(QT_RPATH "${_qt5Core_install_prefix}/lib/")
    set(LIB_RPATH "\${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
    set(RPATH_TARGET "${CURRENT_TARGET_PATH}/Contents/MacOS/${app_name}")

    set(MACDEPLOYQT_COMMAND "${_qt5Core_install_prefix}/bin/macdeployqt")
    set(MACDEPLOYQT_OPTIONS "${CURRENT_TARGET_PATH} -verbose=3 -always-overwrite")

    install(CODE "EXECUTE_PROCESS(COMMAND ${MACDEPLOYQT_COMMAND} ${MACDEPLOYQT_OPTIONS}
                                  WORKING_DIRECTORY \"\${CMAKE_BINARY_DIR}\"
                                  OUTPUT_FILE macdeployqt_exec_${target}.log
                                  ERROR_FILE macdeployqt_exec_${target}.err)")
endfunction()
