include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/3rdparty
  ${CMAKE_SOURCE_DIR}/common
  ${CMAKE_SOURCE_DIR}/ui
  ${CMAKE_BINARY_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}

# ### temporary
  ${CMAKE_SOURCE_DIR}/core
 )

add_definitions(-DGAMMARAY_CLIENT) # temporary, for stuff in ui not properly independent yet

set(gammaray_client_srcs
  main.cpp
  client.cpp
  remotemodel.cpp
  selectionmodelclient.cpp
  objectclient.cpp
  clienttoolmodel.cpp

# ### temporary
  ../core/tools/widgetinspector/widgetinspector.cpp
  ../core/tools/widgetinspector/overlaywidget.cpp
  ../core/tools/widgetinspector/widgetpreviewwidget.cpp
)

qt4_wrap_ui(gammaray_client_srcs
# ### temporary
  ../core/tools/widgetinspector/widgetinspector.ui
)

qt4_automoc(${gammaray_client_srcs})
qt4_add_resources(gammaray_client_srcs ${CMAKE_SOURCE_DIR}/resources/gammaray.qrc)

add_executable(gammaray-client ${gammaray_client_srcs})

target_link_libraries(gammaray-client ${QT_QTCORE_LIBRARIES} ${QT_QTGUI_LIBRARIES} ${QT_QTNETWORK_LIBRARIES} gammaray_common gammaray_ui gammaray_core)

install(TARGETS gammaray-client ${INSTALL_TARGETS_DEFAULT_ARGS})
