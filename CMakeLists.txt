cmake_minimum_required(VERSION 3.5)

project(Bscratcher VERSION 0.1 LANGUAGES CXX)
include(ExternalProject)

set(L15_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/l15)

ExternalProject_Add(libl15
        PREFIX ${L15_PREFIX}
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/l15-core
        CONFIGURE_COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/l15-core/configure --prefix=<INSTALL_DIR> --with-secp256k1=node --enable-plugin-api --with-pic
        BUILD_COMMAND ${MAKE})

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES scratcher_en_US.ts)

set(PROJECT_SOURCES
        main.cpp
        walletwindow.cpp
        walletwindow.h
        newmnemonicdialog.h
        newmnemonicdialog.cpp
        newmnemonicdialog.ui
        ${TS_FILES}
        mnemonicwordlist.hpp
        mnemonicwordlist.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Bscratcher
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET scratcher APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(bscratcher SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Bscratcher
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()


add_dependencies(Bscratcher libl15)
target_link_libraries(Bscratcher PRIVATE Qt${QT_VERSION_MAJOR}::Widgets l15)

target_link_directories(Bscratcher PRIVATE ${L15_PREFIX}/lib)
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/l15-core/contrib/cex
        ${CMAKE_CURRENT_SOURCE_DIR}/l15-core/src/common
        ${CMAKE_CURRENT_SOURCE_DIR}/l15-core/src/core
        ${CMAKE_CURRENT_SOURCE_DIR}/l15-core/node/src/secp256k1/include
        ${CMAKE_CURRENT_SOURCE_DIR}/l15-core/node/src
        ${CMAKE_CURRENT_SOURCE_DIR}/l15-core/node/src/support
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.l2xl.bscratcher)
endif()
set_target_properties(Bscratcher PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Bscratcher
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Bscratcher)
endif()
