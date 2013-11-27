TEMPLATE = app
TARGET = ueyeviewer
CONFIG += debug_and_release
QT += core \
    gui \
    opengl 
HEADERS += src/qsliderex.h \
    src/imageinfodlg.h \
    src/version.h \
    src/properties.h \
    src/cameralist.h \
    src/mainview.h \
    src/eventthread.h \
    src/paintlabel.h \
    src/cvanalizer.h \
    src/detection.h
SOURCES += src/qsliderex.cpp \
    src/imageinfodlg.cpp \
    src/properties.cpp \
    src/cameralist.cpp \
    src/main.cpp \
    src/mainview.cpp \
    src/eventthread.cpp \
    src/paintlabel.cpp \
    src/cvanalizer.cpp \
    src/detection.cpp \
    src/tabsizexs.cpp \
    src/tabcamera.cpp \
    src/tabimage.cpp \
    src/tabsize.cpp \
    src/tabprocessing.cpp \
    src/tabformat.cpp \
    src/tabio.cpp \
    src/tabxs.cpp
FORMS += src/imageinfodlg.ui \
    src/mainview.ui \
    src/properties.ui
RESOURCES += mainview.qrc
DEFINES += __LINUX__ \
    __user__ \
    __cplusplus
LIBS += -lueye_api \
        -lopencv_core \
        -lopencv_imgproc
INCLUDEPATH += include \
    . \
    src
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
