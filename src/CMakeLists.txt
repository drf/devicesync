
add_subdirectory(core)
add_subdirectory(plugins)

set(devicesync_SRCS
   devicesync.cpp
   main.cpp
   devicesyncview.cpp
   ProgressDialog.cpp
   ConnectDialog.cpp
 )

kde4_add_ui_files(devicesync_SRCS 
	devicesyncview_base.ui 
	prefs_base.ui
	progressdialog.ui
	connectdialog.ui
	forceconnectdialog.ui
)

kde4_add_kcfg_files(devicesync_SRCS settings.kcfgc )

kde4_add_executable(devicesync ${devicesync_SRCS})

target_link_libraries(devicesync ${KDE4_KDEUI_LIBS} ${KDE4_KUTILS_LIBS} devicesynccore)

install(TARGETS devicesync DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

install( FILES devicesync.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES devicesync.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES devicesyncui.rc  DESTINATION  ${DATA_INSTALL_DIR}/devicesync )
