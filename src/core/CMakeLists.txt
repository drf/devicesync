
set(dscore_SRCS
    AbstractDeviceInterface.cpp
    AbstractDevice.cpp
    QueueManager.cpp
    ProgressInterface.cpp
)

kde4_add_library(devicesynccore SHARED ${dscore_SRCS})

target_link_libraries(devicesynccore ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS})

set_target_properties(devicesynccore PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION})

install(TARGETS devicesynccore ${INSTALL_TARGETS_DEFAULT_ARGS})

#Install headers
set(dscore_HDRS
    AbstractDeviceInterface.h
    AbstractDevice.h
    QueueManager.h
)

install(FILES ${dscore_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/devicesync)
install(FILES devicesync_plugin.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})