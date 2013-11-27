/*
 * file    properties.h
 * version 0.1
 *
 * properties dialog class declaration for the Qt uEye viewer
 *
 */

#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <QDialog>
#include <QSlider>
#include <QLineEdit>
#include <QSpinBox>
#include <QString>
#include <QLabel>
#include <QTabWidget>
#include <QCheckBox>
#include <QButtonGroup>
#include <QMap>
#include <uEye.h>

#include "mainview.h"
#include "eventthread.h"
#include "ui_properties.h"
#include "qsliderex.h"

class Mainview;

/* Default minimum gain */
#define IS_MIN_GAIN 0
/* Default maximum gain */
#define IS_MAX_GAIN 100

/* Default minimum blacklevel offset */
#define IS_MIN_BL_OFFSET 0
/* Default maximum blacklevel offset */
#define IS_MAX_BL_OFFSET 255
/* default slider check period in msecs */
#define SLIDER_CHECK_PERIOD 500 //ms

/* 
 * uEye camera properties dialog 
 */
class properties : public QDialog, public Ui::_DlgProperties
{
Q_OBJECT

public:
    /*
     * Constructor
     * hCam: Camera handle as returned from libueye_api
     * camInfo: Camera info structure
     * sensorInfo: Sensor info structure
     * bLive: Live mode indicator
     * parent: Qt parent object
     */
    properties (HIDS hCam, CAMINFO camInfo, SENSORINFO sensorInfo, bool bLive, QWidget* parent = 0);
    /* Destructor */
    virtual ~properties ();

public slots:
    /* Received event slot */
    void eventreceived (int event);

    /* Colormode changed slot */
    void OnClickedColormode(int buttonId);

    /* Restore colormode Settings */
    void RestoreColorModeSettings();

    /* On checkbox state change 'Enable OpenMP' */
    void OnOpenMP_Enable(int iNewState);

    virtual void OnRadioWhiteBalance ();
    virtual void OnRadioEdgeEnhancement ();
    virtual void OnButtonBadPixel ();
    virtual void OnSliderChangeFrameRate (int value);
    virtual void OnSliderChangeExposure (int value);
    virtual void OnSpinBoxChangePixelClock (int value);
    virtual void OnSliderChangePixelClock (int value);
    virtual void OnChangePixelClockSetting (int value);
    virtual void OnSliderChangeBlue (int value);
    virtual void OnSliderChangeGreen (int value);
    virtual void OnSliderChangeRed (int value);
    virtual void OnSliderChangeMaster (int value);
    virtual void OnButtonDefault ();

    virtual void checkAutoFps_clicked ();
    virtual void checkAutoExp_clicked ();
    virtual void checkMaxExp_clicked ();
    virtual void checkAutoGain_clicked ();

    virtual void sliderBlackLevel_valueChanged (int value);
    virtual void checkAutoBlacklevel_clicked ();
    virtual void checkBoxSoftwareGamma_clicked ();
    virtual void checkBoxHardwareGamma_clicked ();
    virtual void checkBoxGainBoost_clicked ();

    virtual void tabWidget_currentChanged (QWidget *);
    virtual void OnImageWidthChanged (int);
    virtual void OnImageHeightChanged (int);
    virtual void OnImageXPosChanged (int);
    virtual void OnImageYPosChanged (int);
    //virtual void OnImageChanged();
    virtual void OnRadioHFormat ();
    virtual void OnRadioVFormat ();
    virtual void OnButtonUpDown ();
    virtual void OnButtonLeftRight ();

    virtual void checkEnableTriggerDelay_clicked ();
    virtual void on_pushButtonPollStatus_clicked ();
    virtual void pushButtonWriteEEPROM_clicked ();
    virtual void sliderTriggerDelay_valueChanged (int);
    virtual void OnRadioFlash ();
    virtual void sliderFlashDelay_valueChanged (int);
    virtual void sliderFlashDuration_valueChanged (int);
    virtual void pushButtonGlobalFlashDelay_clicked ();
    virtual void pushButtonClearEEPROM_clicked ();
    virtual void OnRadioColorCorrection_clicked ();

    virtual void changedFormatsLive (int index);
    virtual void changedFormatsTrigger (int index);

    virtual void OnActivateTestImage ();
    virtual void OnChangedTestImage (int);

    virtual void OnMirrorUpDown ();
    virtual void OnMirrorLeftRight ();

    virtual void OnXSAutoFocus ();
    virtual void OnXSBacklightComp ();
    virtual void OnXSFaceDetect ();
    virtual void OnXSAES ();
    virtual void OnXSAGC ();
    virtual void OnXSImgStabilization();

    virtual void OnAutoUpdateCameraCtrls();
    virtual void OnAutoUpdateImageCtrls();
    virtual void OnUpdateLive(bool bLive);

    /*
     virtual void          checkBoxPollStatus_clicked();
     virtual void          sliderTriggerDelay_valueChanged(int);
     virtual void          checkBoxEnableTriggerDelay_clicked();

     virtual void          pushButtonGlobalFlashDelay_clicked();
     virtual void          sliderFlashDelay_valueChanged(int);
     virtual void          sliderFlashDuration_valueChanged(int);

     virtual void          pushButtonWriteEeprom_clicked();
     virtual void          pushButtonClearEeprom_clicked();
     virtual void          radioButtonOff_clicked();
     */

    /* Change frame rate */
    void ChangeFramerate (double value);
    /* Update pixel clock controls */
    void ChangeExposure (double value);

protected:
    /* Show event slot */
    void showEvent (QShowEvent *);

private:
    /* Camera handle as returned from libueye_api */
    HIDS m_hCamera;
    /* Camera info structure */
    CAMINFO m_CamInfo;
    /* Sensor info structure */
    SENSORINFO m_SensorInfo;

    /* Initialization indicator */
    bool m_bInit;
    /* Live operation indicator */
    bool m_bLive;
    /* Refresh page */
    void RefreshPage ();

    /* Update pixel clock controls */
    void UpdatePixelclockCtrls ();
    /* Update frame rate controls */
    void UpdateFramerateCtrls ();
    /* Update exposure controls */
    void UpdateExposureCtrls ();
    /* Update master gain controls */
    void UpdateMasterGainCtrls ();
    /* Update black level controls */
    void UpdateBlackLevelCtrls ();
    /* Update RGB gain controls */
    void UpdateRGBGainCtrls ();
    /* Update feature controls */
    void UpdateFeaturesCtrls ();
    /* Update edge enhancement controls */
    void UpdateEdgeEnhCtrls ();

    /* Update windows size controls */
    void UpdateWindowSizeCtrls ();
    /* Update size and format controls */
    void UpdateSizeFormatCtrls ();

    /*
     * Get the vertical and horizontal size factors
     * pVFactor: pointer to the vFactor storage
     * pHFactor: pointer to the vFactor storage
     */
    int getSizeFactor(int *pVFactor, int *pHFactor);
    /* Update color controls */
    void UpdateColorCtrls ();
    /* Update whitebalance controls */
    void UpdateWBCtrls ();

    /* Update trigger status */
    void UpdateTriggerStatus ();
    /* Update input controls */
    void UpdateInputCtrls ();
    /* Update output controls */
    void UpdateOutputCtrls ();
    /* Update eeprom status */
    void UpdateEepromCtrls ();
    /* Update image format size tab controls */
    void UpdateSizeXSCtrls();
    /* Update special xs feautures tab controls */
    void UpdateXsFeatures();

    /*
     * Retrieve test image name
     * testimage: Testimage identifiert from uEye.h
     */
    QString GetTestImageName (int testimage);
    /*
     * Create test image list depending on camera capabilities
     * images: Number of images
     */
    void populateTestImages (int images);

    /* Tab camera initialization */
    void initTabCamera();
    /* Tab image initialization */
    void initTabImage();
    /* Tab size initialization */
    void initTabSize();
    /* Tab format size initialization */
    void initTabSizeXS();
    /* Tab format initialization */
    void initTabFormat();
    /* Tab io initialization */
    void initTabIo();
    /* Tab processing initialization */
    void initTabProcessing();
    /* Tab xs special features initialization */
    void initTabXS();
    /* Disable all color formats */
    void DisableColorFormats();
    /* Hide the 5x5 color formats */
    void DoNotShow5x5();
    /* Update the format tab */
    void UpdateTabFormat();
    /* Checks if colorformats are supported */
    bool CheckColorFormat(UINT ColorFormat, UINT ConvertType);
    /* Enables the supported color formats */
    void EnableSupportedColorFormats();
    /* Create an exclusive buttongroup with all color format radio buttons */
    void createRadioButtonGroup();
    /* Fill the comboboxes with the available image formats from the camera */
    int fillComboBoxImageFormats();
    /* Select the current camera formats */
    void selectComboImageFormats();

    // pixelclock
    int m_maxPixelclock;
    int m_minPixelclock;
    int m_incPixelclock;
    int m_nPixelclockListIndex;
    int m_nPixelclockList[150];
    int m_nNumberOfSupportedPixelClocks;
    // framerate
    double m_maxFramerate;
    double m_minFramerate;
    double m_incFramerate;
    double m_dblFramerate;
    bool m_bAutoFramerate;
    bool m_bHasAutoFps;
    // exposure time
    double m_maxExposure;
    double m_minExposure;
    double m_incExposure;
    double m_dblExposure;
    bool m_bAutoExposure;
    bool m_bMaxExposure;
    bool m_bHasAutoExposure;

    // edgeenhancement
    int m_weakEdgeEnhancement;
    int m_strongEdgeEnhancement;

    // masterGain
    int m_maxMasterGain;
    int m_minMasterGain;
    int m_incMasterGain;
    int m_nMasterGain;
    bool m_bAutoGain;
    bool m_bHasAutoGain;
    // blacklevel
    int m_maxBlackLevel;
    int m_minBlackLevel;
    int m_incBlackLevel;
    int m_nBlackLevel;
    bool m_bAutoBlackLevel;
    bool m_bHasAutoBlackLevel;
    bool m_bHasManualBlackLevel;
    // rgbGains
    int m_maxRGBGain;
    int m_minRGBGain;
    int m_incRGBGain;
    int m_nRedGain;
    int m_nGreenGain;
    int m_nBlueGain;
    // features
    bool m_bSoftwareGammaSet;
    bool m_bHardwareGammaSet;
    bool m_bHasHardwareGamma;
    bool m_bHasSoftwareGamma;
    bool m_bHotPixelCorrSet;
    bool m_bGainBoostSet;
    bool m_bHasGainBoost;

    // aoi
    int m_maxWidth;
    int m_minWidth;
    int m_incWidth;
    int m_nWidth;
    int m_maxHeight;
    int m_minHeight;
    int m_incHeight;
    int m_nHeight;
    int m_maxXPos;
    int m_minXPos;
    int m_incXPos;
    int m_nXPos;
    int m_maxYPos;
    int m_minYPos;
    int m_incYPos;
    int m_nYPos;
    int m_vFactor;
    int m_hFactor;

    // horizontal format
    bool m_bHasColorBinning;
    bool m_bHasColorSubsampling;

    // trigger delay
    int m_minTriggerDelay;
    int m_maxTriggerDelay;
    int m_incTriggerDelay;
    int m_nTriggerDelay;
    bool m_bEnableTriggerDelaySet;
    // flash delay
    int m_minFlashDelay;
    int m_maxFlashDelay;
    int m_incFlashDelay;
    int m_nFlashDelay;
    // flash duration
    int m_minFlashDuration;
    int m_maxFlashDuration;
    int m_incFlashDuration;
    int m_nFlashDuration;

    EventThread *m_pEvWB;

    QTimer *m_pTimer;

    // Timer automatic update
    QTimer *m_pTimerUpdateCameraCtrl;
    QTimer *m_pTimerUpdateImageCtrl;

    Mainview* m_pDlgMainView;

    QSliderEx* m_pSliderWidthEx;
    QSliderEx* m_pSliderHeightEx;
    QSliderEx* m_pSliderPosXEx;
    QSliderEx* m_pSliderPosYEx;

    // tab format variables
    bool m_bColor;
    QButtonGroup *pbgColorFormat;

};

#endif
