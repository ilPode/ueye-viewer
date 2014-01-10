/*
 * file    mainview.h
 * author  Davide Poderini
 *
 * MainWindow class header
 *
 */

#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <uEye.h>
#include <QMainWindow>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QImage>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QString>
#include <QColor>
#include <QPainter>
#include <QRadioButton>
#include <QGroupBox>
#include <QSize>
#include <QPoint>
#include <QLayout>
#include <QThread>
#include <QMutex>
#include <QUrl>
#include <QRgb>
#include <QCloseEvent>
#include <QTimerEvent>
#include <QWidget>
#include <QProgressBar>
#include <QTimer>
#include <QTime>
#include <QScrollArea>
#include <QScrollBar>
#include <QVector>

#include "ui_mainview.h"
#include "properties.h"
#include "eventthread.h"
#include "cameralist.h"
#include "imageinfodlg.h"
#include "cvanalizer.h"
#include "detection.h"
#include "paintlabel.h"

#ifndef __LINUX__
	#define snprintf sprintf_s
#endif

class properties;

class WorkerThread;

/* Number of image buffer to alloc */
#define IMAGE_COUNT     5

/* Default value for max displayed frames per sec */
#define UPDATE_TICKS    30

/* uEye Image parameter structure */
typedef struct _UEYE_IMAGE
{
    char *pBuf;
    INT nImageID;
    INT nImageSeqNum;
    INT nBufferSize;
} UEYE_IMAGE, *PUEYE_IMAGE;

/* Worker thread job enumeration */
enum eWorkerJob
{
    wj_none = 0,
    wj_cameraopen = 1,
    wj_cameraopenlive = 2
};

/* Image buffer properties structure */
struct sBufferProps
{
    int width;
    int height;
    int colorformat;
    int bitspp;
    QImage::Format imgformat;
    QRgb *pRgbTable;
    int tableentries;
    int imageformat;

};

/* Camera feature properties structure */
struct sCameraProps
{
    bool bUsesImageFormats;
    int nImgFmtNormal;
    int nImgFmtTrigger;

};


/* 
 * MainWindow class declaration 
 */
class Mainview:public QMainWindow, public Ui::MainviewClass
{
Q_OBJECT

public:
    /*
     * Standard constructor for the MainWindow object
     * parent: Qt parent object
     * strStartID: Select the device ID to open
     */
    Mainview (QWidget *parent = 0, QString strStartID = "");
    /* Standard destructor */
    virtual ~Mainview ();

    /* workerthread run function */
    void run();
    /*
     * Returns the number of an image buffer
     * pbuf: image buffer pointer
     */
    INT _GetImageNum (char* pbuf);
    /* Frees all image buffers */
    void _FreeImages ();
    /* Clears all image buffer contents */
    void _EmptyImages ();
    /*
     * Returns the ID of an image buffer
     * pbuf: image buffer pointer
     */
    INT _GetImageID (char* pbuf);
    /*
     * Creates all needed imagebuffers,
     * returns true if successful, false otherwise
     */
    bool _AllocImages ();
    /* Returns the curretn camera handle */
    HIDS getCameraHandle() { return m_hCamera; }
    /* Returns the bits per pixel for each color format */
    int GetBitsPerPixel(int colormode);

public slots:
    /* Shows the about dialog */
    void about ();

    /* uEye event receive slot function */
    void eventreceived (int event);

    /* Opens/closes the ueye cameras */
    void onOpenCamera();
    void onOpenCameraLive();
    void onCloseCamera();

    /* Setups the capture process */
    int SetupCapture();

    /* Starts/stops ueye live capture */
    void onLive ();

    /* Captures a snapshot */
    void onSnapshot ();

    /* Save the actual uEye image */
    void onSaveImage ();

    /* Loads an image and display it */
    void onLoadImage();
    void onLoadImageCam();

    /* Resets the camera to its default values */
    void onResetCamera();

    /*
     * Imports a camera parameter file
     * action: triggered menu action
     */
    void onLoadParameter (QAction* action);

    /*
     * Exports the actual camera parameter to a file
     * action: triggered menu action
     */
    void onSaveParameter (QAction* action);

    /* Prepares the LoadParameter Menu */
    void onPrepareLoadParameter();

    /* Shows the camera property dialog */
    void onProperties ();

    /* Use external triggers for capture images */
    void onUseTrigger ();

    /* Enable/Disbale the API error report */
    void onApiErrorReport();

    /* Show/hide the Image Info Dialog */
    void onImageInfo();

    /* Imageinfo timeout slot */
    void onImageInfoUpdate();

    /* Enable/disable display rate limit */
    void onLimitDisplayRate();
    
    /* Fit displayed image to window */
    void onFitWindow(bool bFit);
    
    /* Update shape list */
    void onUpdateList();
    
    /* Updates the units conversion factor */
    void onUpdateScale();
    
protected:
    void closeEvent (QCloseEvent * e);
    
    /*
     * Overloads parents 'paintEvent' function
     * e: pointer to Qt paint event
     */
    void paintEvent (QPaintEvent *e);
    
    /* Clears the statusbar info  */
    void ClearStatusbarInfo();

    /*
     * Updates the fps display in the statusbar
     * fps: new frames per second value
     */
    void UpdateFps (double fps);
    
    /* Updates the frame counter display in the statusbar */
    void UpdateCounters ();
    
    /* Updates the mainwindow control elements */
    void UpdateControls ();

private:
    void adjustScrollBar(QScrollBar *scrollBar, double factor);

    /* Creates the mainwindow status bar */
    void createStatusBar ();

    /* uEye image process function */
    void ProcessFrame ();

    /*
     * uEye image draw/display function
     * pBuffer: pointer to image buffer to draw
     * image: QImage to draw 
     */
    void DrawImage (char *pBuffer);
    void DrawImage(QImage & image);

    /*
     * Initialize the worker thread,
     * returns true if success, otherwise false
     */
    bool prepareWorkThread();

    /*
     * Function to call when a camera was initialized through the work thread to
     * finish the initializing work, returns ids error code (int)
     */
    int CameraInitialized();

    int openCamera (bool bStartLive);

    int searchDefImageFormats(int suppportMask);
    
    /* Saves and reads the position and window states */
    void writeSettings();
    void readSettings();

private slots:
    /* Callback slots when worker thread started or stopped */
    void workThreadStarted();
    void workThreadFinished();

    /* Update progressbar slot */
    void updateProgress();

private:
    /* Pointer on a camera list class object */
    CameraList *pDlgCamerList;
    /* Pointer on a imageinfodlg class object */
    imageinfodlg *m_pImageInfoDlg;
    /* UEye frame buffer array */
    UEYE_IMAGE m_Images[IMAGE_COUNT];
    /* UEye camera handle */
    HIDS m_hCamera;
    /* UEye camera info structmember */
    CAMINFO m_CamInfo; // Camera (Board)info
    /* UEye CameraList struct member */
    UEYE_CAMERA_INFO m_CamListInfo;
    /* UEye sensor info struct member */
    SENSORINFO m_SensorInfo; // SensorInfo
    /* Holds the uEye sensor width */
    int m_nWidth;
    /* Holds the uEye sensor height */
    int m_nHeight;
    /* Holds the current display scale format */
    bool m_bReady;
    char *m_pLastBuffer;

    /* Used color mode */
    int m_nColorMode;
    /* Colorindex table for monochrom pictures */
    QRgb m_table[256];
    /* Display images switch */
    bool m_bDisplayImage;

    /* Capture Status */
    bool m_bLive;               // live indicator
    unsigned long nReceived;    // current received frames
    unsigned long nFailed;      // current count of failed transfers
    unsigned long nDisplayed;   // current displayed images

    /* DEVICE_REMOVE Event waiting thread member */
    EventThread *m_pEvDevremove;
    /* DEVICE_NEW Event waiting thread member */
    EventThread *m_pEvDevnew;
    /* STATUS_CHANGED Event waiting thread member */
    EventThread *m_pEvDevStateChanged;
    /* FRAME Event waiting thread member */
    EventThread *m_pEvFrame;
    /* TRANSFER_FAILED Event waiting thread member */
    EventThread *m_pEvTransferFailed;

    /* Pointer to the child view object */
    PaintLabel *m_imageLabel;
    QScrollArea *m_scrollArea;

    /* Statusbar text labels */
    QLabel *textStatus;
    QLabel *textStatus_2;
    QLabel *textStatus_3_0;
    QLabel *textStatus_3_1;
    QLabel *textStatus_3_2;
    QLabel *textStatus_4;

    /* Workerthread jobnumber */
    int m_workthread_jobNumber;

    /* Workerthread jobnumber */
    eWorkerJob m_eWorkthreadJob;

    /* Workerthread return value */
    int m_workthreadReturn;

    /* Worker thread member */
    WorkerThread *m_workThread;

    /* Progress indicator */
    QProgressBar *m_pProgBar;

    /* Progress indicator text */
    QLabel *m_pProgText;

    /* Flag to show if automatic fw upload is used */
    bool m_bAutomaticSEStarterUpload;

    /* Misc timer */
    QTimer *m_pTimer;

    /* Property dialog */
    properties *m_pPropertiesDlg;

    /* Misc time variable */
    QTime m_Time;
    int m_nUpdateTicks;

    /* Image analizer timer */
    DetectionThread * m_pDetThread;

    /* QFileDialog's state */
    QString loadDialogPath;
    QString saveDialogPath;

public:
    struct sBufferProps m_BufferProps;
    struct sCameraProps m_CameraProps;


};

/*
 * Workerthread class
 */

class WorkerThread : public QThread {
  Q_OBJECT

public:
  WorkerThread(Mainview *mainWindow)
    : mainWindow(mainWindow){};
  virtual ~WorkerThread(){};

  void run() {mainWindow->run();}

protected:
  Mainview *mainWindow;

};

#endif
