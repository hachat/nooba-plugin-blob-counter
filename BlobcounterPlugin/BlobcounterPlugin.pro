######################################################################
# Automatically generated by qmake (2.01a) Fri Aug 23 13:22:33 2013
######################################################################
CONFIG(debug, debug|release):    message(Debug build!)
CONFIG(release, debug|release):    message(Release build!)

QT += core gui

TARGET = blobcounterPlugin

CONFIG(debug, debug|release): TARGET = BlobcounterPlugin_Debug
CONFIG(release, debug|release): TARGET = BlobcounterPlugin_Release

TEMPLATE = lib
CONFIG += dll

DEFINES += BLOBCOUNTERPLUGIN_LIB

# Input
HEADERS += blobcounterplugin.h blobcounterplugin_global.h \
    detectedevent.h \
    linecrosscountnode.h \
    linecrossingnode.h



SOURCES += blobcounterplugin.cpp \
    featurenode.cpp \
    detectedevent.cpp \
    linecrosscountnode.cpp \
    linecrossingnode.cpp


CONFIG(debug, debug|release): DESTDIR = $$PWD/../../../NoobaVSS_build/NoobaFE/Debug/plugins/
CONFIG(release, debug|release): DESTDIR = $$PWD/../../../NoobaVSS_build/NoobaFE/Release/plugins/

# include opencv
unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
    # remove sym links to the shared lib from the DESTDIR
    QMAKE_POST_LINK = find $$DESTDIR -maxdepth 1 -type l -exec rm -f {} \\;
}

CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../NoobaVSS_build/NoobaPluginAPI/Debug/ -lNoobaPluginAPId
CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../NoobaVSS_build/NoobaPluginAPI/Release/ -lNoobaPluginAPI

INCLUDEPATH += $$PWD/../../../NoobaVSS/NoobaPluginAPI/
DEPENDPATH += $$PWD/../../../NoobaVSS/NoobaPluginAPI/
