# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------

TEMPLATE = app
TARGET = shpviewer
DESTDIR = ../Win32/Release
QT += core opengl gui widgets openglextensions
CONFIG += release
DEFINES += _UNICODE _ENABLE_EXTENDED_ALIGNED_STORAGE WIN64 QT_DLL QT_WIDGETS_LIB QT_OPENGL_LIB QT_OPENGLEXTENSIONS_LIB
INCLUDEPATH += ./GeneratedFiles \
    . \
    ./GeneratedFiles/$(ConfigurationName)
LIBS += -lopengl32 \
    -lglu32
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/$(ConfigurationName)
OBJECTS_DIR += release
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(shpviewer.pri)
