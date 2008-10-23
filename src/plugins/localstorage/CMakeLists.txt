set(localstorageplugin_SRCS
    LocalStorage.cpp
    LocalStorageDevice.cpp
)

kde4_add_plugin(devicesync_localstorageplugin ${localstorageplugin_SRCS})

target_link_libraries(devicesync_localstorageplugin ${KDE4_KDECORE_LIBS} ${KDE4_KIO_LIBS} devicesynccore)
install(TARGETS devicesync_localstorageplugin DESTINATION  ${PLUGIN_INSTALL_DIR})
install(FILES devicesync_localstorage.desktop DESTINATION ${SERVICES_INSTALL_DIR})