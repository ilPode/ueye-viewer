Ueye Viewer
==========
Ueye viewer is a very simple viewer for ueye cameras with some basic shape recognition capabilities, based on the sample program included in the IDS Software Suite for linux.

Download the code
    git clone git@github.com:ilpode/ueyeviewer.git

Building on Linux
    cd ueye-viewer
    qmake
    make

Installing on Windows
There is zip package with a compiled win32 binary and all the needed libraries (except for the Ueye libraries) in ueye-viewer/win32-bin/ueyeviewer.zip. 

Building on Windows
If you use minGW (http://mingw.org/), then the procedure is the same on windows, just remember to adjust the LIB and INCLUDE path in uEyeViewer.pro for your system.
If you use Visual Studio you can use the command
    qmake -tp vcapp
to generate a Project file.

Dependens on:
- IDS Ueye
http://en.ids-imaging.com/download-ueye.html
- Qt4 
http://qt-project.org
- OpenCV
http://opencv.org/
