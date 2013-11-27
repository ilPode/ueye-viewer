/*
 * file    properties.cpp
 * version 0.1
 *
 * properties dialog class implementation for the Qt uEye viewer
 *
 */

#include "properties.h"

properties::properties (HIDS hCam,CAMINFO camInfo,SENSORINFO sensorInfo,bool bLive,QWidget* parent) :
            QDialog (parent),
            m_hCamera (hCam),
            m_bLive (bLive),
            // pixelclock
            m_maxPixelclock (0),
            m_minPixelclock (0),
            m_incPixelclock (0),
            m_nPixelclockListIndex (0),
            // framerate
            m_maxFramerate (0.0),
            m_minFramerate (0.0),
            m_incFramerate (0.0),
            m_dblFramerate (0.0),
            m_bAutoFramerate (FALSE),
            m_bHasAutoFps (FALSE),
            // exposure time
            m_maxExposure (0.0),
            m_minExposure (0.0),
            m_incExposure (0.0),
            m_dblExposure (0.0),
            m_bAutoExposure (FALSE),
            m_bMaxExposure (FALSE),
            m_bHasAutoExposure (FALSE),
            // masterGain
            m_maxMasterGain (0),
            m_minMasterGain (0),
            m_incMasterGain (0),
            m_nMasterGain (0),
            m_bAutoGain (FALSE),
            m_bHasAutoGain (FALSE),
            // blacklevel
            m_maxBlackLevel (0), m_minBlackLevel (0), m_incBlackLevel (0),
            m_nBlackLevel (0),
            m_bAutoBlackLevel (FALSE),
            m_bHasAutoBlackLevel (FALSE),
            m_bHasManualBlackLevel (FALSE),
            // rgbGains
            m_maxRGBGain (0), m_minRGBGain (0), m_incRGBGain (0),
            m_nRedGain (0),
            m_nGreenGain (0),
            m_nBlueGain (0),
            // features
            m_bSoftwareGammaSet (FALSE), m_bHardwareGammaSet (FALSE),
            m_bHasHardwareGamma (FALSE), m_bHotPixelCorrSet (FALSE), m_bGainBoostSet (FALSE),
            m_bHasGainBoost (FALSE),
            // aoi
            m_vFactor (1),
            m_hFactor (1)
{
    setupUi (this);

    QString strTemp, strTemp1;

    m_bInit = TRUE;
    m_pDlgMainView = (Mainview*)parent;

    memcpy (&m_CamInfo, &camInfo, sizeof(camInfo));
    memcpy (&m_SensorInfo, &sensorInfo, sizeof(sensorInfo));

    UEYE_AUTO_INFO autoInfo;
    if (is_GetAutoInfo (m_hCamera, &autoInfo) == IS_SUCCESS)
    {
        m_bHasAutoExposure = autoInfo.AutoAbility & AC_SHUTTER;
        m_bHasAutoFps = m_bHasAutoExposure;
        m_bHasAutoGain = autoInfo.AutoAbility & AC_GAIN;
    }
    else
    {
        m_bHasAutoFps = FALSE;
        m_bHasAutoExposure = FALSE;
        m_bHasAutoGain = FALSE;
    }

    /*! \note if camera uses IMAGE FORMATS instead of continuous AOI size  we have to disable the
     * normal size controls */
    INT nAOISupported = 0;
    INT ret = 0;
    if ((ret=is_ImageFormat(m_hCamera, IMGFRMT_CMD_GET_ARBITRARY_AOI_SUPPORTED, (void*)&nAOISupported,
                       sizeof(nAOISupported))) != IS_SUCCESS)
    {
        qDebug("ERROR: Properties -> initTabSize: is_ImageFormat returns %d", ret);
    }
    if (nAOISupported == 0)
    {
        // hide the whole size tab page
        tabWidget->removeTab(tabWidget->indexOf(Size));
        // init SizeXS tab page
        initTabSizeXS();
        initTabXS();
    }
    else
    {
        // hide the image format tab page
        tabWidget->removeTab(tabWidget->indexOf(SizeXS));
        tabWidget->removeTab(tabWidget->indexOf(XsFeatures));
        // init the size (aoi) tab parameter
        initTabSize();
    }

    m_pTimerUpdateCameraCtrl = new QTimer(this);
    m_pTimerUpdateImageCtrl = new QTimer(this);

    // init the IO tab page
    initTabIo();

    // init the processing tab
    initTabProcessing();

    // init the image tab page
    initTabImage();

    // init the camera tab page
    initTabCamera();

    /* init the format tab page
     * \note Before we can do this we have to get the sensorinfo
     */
    initTabFormat();

    m_bInit = FALSE;
}

properties::~properties ()
{
    if (m_pEvWB)
    {
        m_pEvWB->stop ();
        if (m_pEvWB->wait (250) == FALSE)
            m_pEvWB->terminate ();
        delete m_pEvWB;
        m_pEvWB = NULL;
    }
}

// -------------------  showEvent  -----------------------
//
void properties::RefreshPage ()
{
    int curindex = tabWidget->currentIndex ();

    if (curindex == tabWidget->indexOf(Timing))
    {
        // CAMERA
        UpdatePixelclockCtrls ();
        UpdateExposureCtrls ();
        /* UpdateFramerateCtrls depends on the results of UpdateExposureCtrls. */
        UpdateFramerateCtrls ();
    }
    else if (curindex == tabWidget->indexOf(Image))
    {
        // IMAGE
        UpdateMasterGainCtrls ();
        UpdateBlackLevelCtrls ();
        UpdateRGBGainCtrls ();
        UpdateFeaturesCtrls ();
        UpdateEdgeEnhCtrls ();
    }
    else if (curindex == tabWidget->indexOf(Size))
    {
        // SIZE
        UpdateWindowSizeCtrls ();
        UpdateSizeFormatCtrls ();
    }
    else if (curindex == tabWidget->indexOf(SizeXS))
    {
        // SIZEXS
        UpdateSizeXSCtrls();

    }
    else if (curindex == tabWidget->indexOf(Format))
    {
        // FORMAT
        UpdateTabFormat();
    }
    else if (curindex == tabWidget->indexOf(Processing))
    {
        // IMAGEPROCESSING
        UpdateColorCtrls ();
        UpdateWBCtrls ();
    }
    else if (curindex == tabWidget->indexOf(XsFeatures))
    {
        // XS special features
        UpdateXsFeatures ();
    }
    else if (curindex == tabWidget->indexOf(InOut))
    {
        // I/O
        UpdateTriggerStatus ();
        UpdateInputCtrls ();
        UpdateOutputCtrls ();
        UpdateEepromCtrls ();
    }
    else
    {
        // asume first page (or better all??)

    } // end switch(tabWidget->currentIndex())
}

// -------------------  showEvent  -----------------------
//
void properties::showEvent (QShowEvent *)
{
    RefreshPage ();
}

// -------------------  tabWidget_currentChanged  -----------------------
//
void properties::tabWidget_currentChanged (QWidget * widget)
{
    Q_UNUSED(widget);
    RefreshPage ();
}

// -------------------  OnButtonDefault  -----------------------
//
void properties::OnButtonDefault ()
{
    if (m_bInit || !m_hCamera)
        return;

    INT colorMode;
    bool wasLive;
    // keep color format
    colorMode = is_SetColorMode (m_hCamera, IS_GET_COLOR_MODE);
    wasLive = (is_CaptureVideo (m_hCamera, IS_GET_LIVE) != 0);
    is_StopLiveVideo (m_hCamera, IS_WAIT);

    // restore all defaults
    is_ResetToDefault (m_hCamera);
    is_SetColorMode (m_hCamera, colorMode);

    // reset flags
    m_bEnableTriggerDelaySet = false;
    m_bMaxExposure = false;
    chkOpenMP_Enable->setChecked(false);

    // reset colormode radio button
    RestoreColorModeSettings();

    RefreshPage ();

    m_pDlgMainView->SetupCapture();

    if (wasLive)
        is_CaptureVideo (m_hCamera, IS_DONT_WAIT);
}

void properties::eventreceived (int event)
{
    if(isVisible())
    {
        switch (event)
        {
        case IS_SET_EVENT_WB_FINISHED:
            //printf ("event WB finished detected\n");
            UpdateWBCtrls ();
            break;

        default:
            break;
        }
    }
}

void properties::OnUpdateLive(bool bLive)
{
    m_bLive = bLive;
}
