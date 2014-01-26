project(wizservice_plugin)

set(wizservice_HEADERS
    wizservice_global.h
    wizserviceplugin.h
)

set(wizservice_SOURCES
    wizserviceplugin.cpp
)

set(wizservice_FORMS
)

set(wizservice_SPEC
    WizService.pluginspec
)

include(QtChooser)
qt_add_ui(wizservice_FORM_HEADERS ${wizservice_FORMS})
add_library(wizservice SHARED ${wizservice_SOURCES} ${wizservice_HEADERS} ${wizservice_FORM_HEADERS} ${wizservice_SPEC})
qt_use_modules(wizservice)
qt_suppress_warnings(wizservice)

set_target_properties(wizservice PROPERTIES LIBRARY_OUTPUT_NAME "WizService")
add_definitions(-DWIZSERVICE_LIBRARY)

include_directories(
    ${libs_SOURCE_DIR}
)
target_link_libraries(wizservice extensionsystem)

add_custom_command(TARGET wizservice POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/WizService.pluginspec ${WIZNOTE_PLUGIN_DIR})

install(FILES ${coreplugin_SPEC} DESTINATION lib/wiznote/plugins)
install(TARGETS wizservice DESTINATION lib/wiznote/plugins)
