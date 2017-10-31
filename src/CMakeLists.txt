include(GNUInstallDirs)

include_directories(${CLANG_INCLUDE_DIRS})
add_definitions(${CLANG_DEFINITIONS})
add_definitions(-std=c++11)

add_library(glutamat SHARED
    GlutamatAstAction.cpp
    GlutamatAstConsumer.cpp
    SingletonAstVisitor.cpp
    main.cpp
)
set_target_properties(glutamat PROPERTIES
    LINKER_LANGUAGE CXX
    LINK_FLAGS "-Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/ExportedSymbolsList"
    SOVERSION 1.0)

install(TARGETS glutamat
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
