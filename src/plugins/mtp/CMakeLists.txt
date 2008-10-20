set(mtpplugin_SRCS
    MtpInterface.cpp
    MtpDevice.cpp
)

kde4_add_plugin(devicesync_mtpplugin ${mtpplugin_SRCS})

target_link_libraries(devicesync_mtpplugin ${KDE4_KDEUI_LIBS} devicesynccore)
install(TARGETS devicesync_mtpplugin DESTINATION  ${PLUGIN_INSTALL_DIR})
install(FILES devicesync_mtp.desktop DESTINATION ${SERVICES_INSTALL_DIR})