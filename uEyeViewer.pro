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

unix:DEFINES += __LINUX__ \
DEFINES += __user__ \
    __cplusplus

win32:LIBS += -L"C:\opencv\build\x86\mingw\lib" \
	-L"C:\windows\system32" \
	-lopencv_imgproc247 \ 
	-lopencv_core247 \
      	-lopencv_highgui247
unix:LIBS += -lopencv_core \
        -lopencv_imgproc \
        -lopencv_highgui
LIBS += -lueye_api 

win32:INCLUDEPATH += "C:\opencv\build\include" \
	"C:\Program Files\IDS\uEye\Develop\include"
INCLUDEPATH += 	. src

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
