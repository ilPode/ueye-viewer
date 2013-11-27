/*
 * file    tabxs.cpp
 * version 0.1
 *
 * properties tab widget for special ueye xs camera features
 *
 */

#include "properties.h"

void properties::initTabXS()
{

    connect (cB_xsAutoFocusEnable, SIGNAL(clicked()), this, SLOT(OnXSAutoFocus()));
    connect (cB_xsBacklightCompEnable, SIGNAL(clicked()), this, SLOT(OnXSBacklightComp()));
    connect (cB_xsFaceDetectEnable, SIGNAL(clicked()), this, SLOT(OnXSFaceDetect()));
    connect (cB_xsAES, SIGNAL(clicked()), this, SLOT(OnXSAES()));
    connect (cB_xsAGC, SIGNAL(clicked()), this, SLOT(OnXSAGC()));
    connect (cB_xsImgStab, SIGNAL(clicked()), this, SLOT(OnXSImgStabilization()));
}

void properties::UpdateXsFeatures()
{
    int ret = 0;
    // get AutoFocus Enable
    UINT bEnable = 0;
    double dEnable = 0;
    ret = is_Focus (m_hCamera, FOC_CMD_GET_AUTOFOCUS_ENABLE, &bEnable, sizeof(UINT));
    if (ret != IS_SUCCESS)
        qDebug("error: is_Focus (FOC_CMD_GET_AUTOFOCUS_ENABLE) returned %d", ret);
    else
        cB_xsAutoFocusEnable->setChecked ((bool)bEnable);

    // get backlight compensation enable
    ret = is_SetAutoParameter(m_hCamera, IS_GET_SENS_AUTO_BACKLIGHT_COMP, &dEnable, NULL);
    if (ret != IS_SUCCESS)
        qDebug("error: is_SetAutoParameter (IS_GET_SENS_AUTO_BACKLIGHT_COMP) returned %d", ret);
    else
        cB_xsBacklightCompEnable->setChecked ((bool)dEnable);

    // get Face Detection enable
    ret = is_FaceDetection(m_hCamera, FDT_CMD_GET_ENABLE, &bEnable, sizeof(UINT));
    if (ret != IS_SUCCESS)
        qDebug("error: is_FaceDetection (FDT_CMD_GET_ENABLE) returned %d", ret);
    else
        cB_xsFaceDetectEnable->setChecked ((bool)bEnable);

    // get Sensor AES enable
    ret = is_SetAutoParameter(m_hCamera, IS_GET_ENABLE_AUTO_SENSOR_SHUTTER, &dEnable, NULL);
    if (ret != IS_SUCCESS)
        qDebug("error: is_SetAutoParameter (IS_GET_ENABLE_AUTO_SENSOR_SHUTTER) returned %d", ret);
    else
        cB_xsAES->setChecked ((bool)dEnable);

    // get Sensor AGC enable
    ret = is_SetAutoParameter(m_hCamera, IS_GET_ENABLE_AUTO_SENSOR_GAIN, &dEnable, NULL);
    if (ret != IS_SUCCESS)
        qDebug("error: is_SetAutoParameter (IS_GET_ENABLE_AUTO_SENSOR_GAIN) returned %d", ret);
    else
        cB_xsAGC->setChecked ((bool)dEnable);

    // get sensor Image stabilization
    ret = is_ImageStabilization(m_hCamera, IMGSTAB_CMD_GET_ENABLE, &bEnable, sizeof(UINT));
    if (ret != IS_SUCCESS)
        qDebug("error: is_ImageStabilization (IMGSTAB_CMD_GET_ENABLE) returned %d", ret);
    else
        cB_xsImgStab->setChecked ((bool)bEnable);

}

void properties::OnXSAutoFocus()
{
    BOOL bEnable = cB_xsAutoFocusEnable->isChecked ();
    int ret = 0;
    if (bEnable)
    {
        ret = is_Focus (m_hCamera, FOC_CMD_SET_ENABLE_AUTOFOCUS, NULL, 0);
        if (ret != IS_SUCCESS)
        {
            qDebug("error: is_Focus (FOC_CMD_SET_ENABLE_AUTOFOCUS) returned %d", ret);
            cB_xsAutoFocusEnable->setChecked (false);
        }
    }
    else
    {
        ret = is_Focus (m_hCamera, FOC_CMD_SET_DISABLE_AUTOFOCUS, NULL, 0);
        if (ret != IS_SUCCESS)
        {
            qDebug("error: is_Focus (FOC_CMD_SET_DISABLE_AUTOFOCUS) returned %d", ret);
            cB_xsAutoFocusEnable->setChecked (true);
        }
    }

}

void properties::OnXSAES()
{
    BOOL bEnable = cB_xsAES->isChecked ();
    int ret = 0;
    double dEnable = (bEnable)? 1:0;
    ret = is_SetAutoParameter(m_hCamera, IS_SET_ENABLE_AUTO_SENSOR_SHUTTER, &dEnable, NULL);
    if (ret != IS_SUCCESS)
    {
        qDebug("error: is_SetAutoParameter (IS_SET_ENABLE_AUTO_SENSOR_SHUTTER) returned %d", ret);
        cB_xsAES->setChecked (!dEnable);
    }
}

void properties::OnXSAGC()
{
    BOOL bEnable = cB_xsAGC->isChecked ();
    int ret = 0;
    double dEnable = (bEnable)? 1:0;
    ret = is_SetAutoParameter(m_hCamera, IS_SET_ENABLE_AUTO_SENSOR_GAIN, &dEnable, NULL);
    if (ret != IS_SUCCESS)
    {
        qDebug("error: is_SetAutoParameter (IS_SET_ENABLE_AUTO_SENSOR_GAIN) returned %d", ret);
        cB_xsAGC->setChecked (!dEnable);
    }
}

void properties::OnXSBacklightComp ()
{
    BOOL bEnable = cB_xsBacklightCompEnable->isChecked ();
    int ret = 0;
    double dEnable = (bEnable)? 1:0;
    ret = is_SetAutoParameter(m_hCamera, IS_SET_SENS_AUTO_BACKLIGHT_COMP, &dEnable, NULL);
    if (ret != IS_SUCCESS)
    {
        qDebug("error: is_SetAutoParameter (IS_SET_SENS_AUTO_BACKLIGHT_COMP) returned %d", ret);
        cB_xsBacklightCompEnable->setChecked (!dEnable);
    }
}

void properties::OnXSFaceDetect ()
{
    BOOL bEnable = cB_xsFaceDetectEnable->isChecked ();
    UINT nMaxFaces = 3;
    int ret = 0;
    if (bEnable)
    {
        ret = is_FaceDetection(m_hCamera, FDT_CMD_SET_INFO_MAX_NUM_OVL, &nMaxFaces, sizeof(UINT));
        if (ret != IS_SUCCESS)
        {
            qDebug("error: is_FaceDetection (FDT_CMD_SET_INFO_MAX_NUM_OVL) returned %d", ret);
        }
        ret = is_FaceDetection(m_hCamera, FDT_CMD_SET_ENABLE, NULL, 0);
        if (ret != IS_SUCCESS)
        {
            qDebug("error: is_FaceDetection (FDT_CMD_SET_ENABLE) returned %d", ret);
            cB_xsFaceDetectEnable->setChecked (false);
        }
    }
    else
    {
        ret = is_FaceDetection (m_hCamera, FDT_CMD_SET_DISABLE, NULL, 0);
        if (ret != IS_SUCCESS)
        {
            qDebug("error: is_FaceDetection (FDT_CMD_SET_DISABLE) returned %d", ret);
            cB_xsFaceDetectEnable->setChecked (true);
        }
    }
}

void properties::OnXSImgStabilization()
{
    BOOL bEnable = cB_xsImgStab->isChecked ();
    int ret = 0;
    if (bEnable)
    {
        ret = is_ImageStabilization(m_hCamera, IMGSTAB_CMD_SET_ENABLE, NULL, 0);
        if (ret != IS_SUCCESS)
        {
            qDebug("error: is_ImageStabilization (IMGSTAB_CMD_SET_ENABLE) returned %d", ret);
            cB_xsImgStab->setChecked (false);
        }
    }
    else
    {
        ret = is_ImageStabilization (m_hCamera, IMGSTAB_CMD_SET_DISABLE, NULL, 0);
        if (ret != IS_SUCCESS)
        {
            qDebug("error: is_ImageStabilization (IMGSTAB_CMD_SET_DISABLE) returned %d", ret);
            cB_xsImgStab->setChecked (true);
        }
    }
}
