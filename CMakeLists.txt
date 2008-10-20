project(devicesync)

set( CMAKE_MODULE_PATH    ${CMAKE_MODULE_PATH}
                          ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules )

find_package(KDE4 REQUIRED)
include (KDE4Defaults)

include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} src/core )

macro_optional_find_package(Mtp)
macro_log_feature( MTP_FOUND "libmtp" "Necessary to build Mtp Plugin" "http://libmtp.sourceforge.net/" FALSE "0.3.0" "")

add_subdirectory( doc )
add_subdirectory( src )
add_subdirectory( icons )

macro_display_feature_log()
