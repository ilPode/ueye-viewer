/*
 * file    tabsizexs.cpp
 * version 0.1
 *
 * properties tab widget for camera format size operations
 *
 */

#include "properties.h"

void properties::initTabSizeXS()
{
    fillComboBoxImageFormats();
    selectComboImageFormats();

    /* connect the combobox signals only after the combobox creation */
    connect(cB_FormatsLive, SIGNAL(currentIndexChanged(int)), this, SLOT(changedFormatsLive(int)));
    connect(cB_FormatsTrigger, SIGNAL(currentIndexChanged(int)), this, SLOT(changedFormatsTrigger(int)));

    connect (cB_Mirror_lr, SIGNAL(clicked()), this, SLOT(OnMirrorLeftRight()));
    connect (cB_Mirror_ud, SIGNAL(clicked()), this, SLOT(OnMirrorUpDown()));
}


/*! \brief fill the comboboxes with available image format of a camera
 *
*/
int properties::fillComboBoxImageFormats()
{
    int ret = IS_SUCCESS;
    int nNumber;
    IMAGE_FORMAT_LIST *pFormatList;
    QString strFormat;

    if ((ret=is_ImageFormat(m_hCamera, IMGFRMT_CMD_GET_NUM_ENTRIES, (void*)&nNumber, sizeof(nNumber))) == IS_SUCCESS)
    {
        int i = 0;
        int nSize = sizeof(IMAGE_FORMAT_LIST) + (nNumber - 1) * sizeof(IMAGE_FORMAT_LIST);
        pFormatList = (IMAGE_FORMAT_LIST*)(new char[nSize]);
        pFormatList->nNumListElements = nNumber;
        pFormatList->nSizeOfListEntry = sizeof(IMAGE_FORMAT_INFO);

        if((ret=is_ImageFormat(m_hCamera, IMGFRMT_CMD_GET_LIST, (void*)pFormatList, nSize)) == IS_SUCCESS)
        {
            for(i=0; i<nNumber; i++)
            {
                // Freerun or single snaphot
                if (((pFormatList->FormatInfo[i].nSupportedCaptureModes & CAPTMODE_FREERUN) &&
                     (pFormatList->FormatInfo[i].nSupportedCaptureModes & CAPTMODE_SINGLE)) ||
                     (pFormatList->FormatInfo[i].nSupportedCaptureModes & CAPTMODE_TRIGGER_SOFT_SINGLE)
                   )
                {
                    strFormat = QString("%1 x %2").arg(pFormatList->FormatInfo[i].nWidth).arg(pFormatList->FormatInfo[i].nHeight);

                    if (pFormatList->FormatInfo[i].nSupportedCaptureModes & CAPTMODE_TRIGGER_SOFT_SINGLE)
                    {
                        if ((pFormatList->FormatInfo[i].nWidth == 3264) &&
                            (pFormatList->FormatInfo[i].nHeight == 2448))
                        {
                            strFormat += " - 8M";
                        }
                        else if ((pFormatList->FormatInfo[i].nWidth == 3264) &&
                            (pFormatList->FormatInfo[i].nHeight == 2176))
                        {
                            strFormat += " - 8M 3:2";
                        }
                        else if ((pFormatList->FormatInfo[i].nWidth == 3264) &&
                            (pFormatList->FormatInfo[i].nHeight == 1836))
                        {
                            strFormat += " - 8M 16:9";
                        }
                        else if ((pFormatList->FormatInfo[i].nWidth == 2592) &&
                            (pFormatList->FormatInfo[i].nHeight == 1944))
                        {
                            strFormat += " - 5M";
                        }
                        else if ((pFormatList->FormatInfo[i].nWidth == 2048) &&
                            (pFormatList->FormatInfo[i].nHeight == 1536))
                        {
                            strFormat += " - 3M";
                        }
                        else if ((pFormatList->FormatInfo[i].nWidth == 1920) &&
                            (pFormatList->FormatInfo[i].nHeight == 1080))
                        {
                            strFormat += " - HD 16:9";
                        }
                        else if ((pFormatList->FormatInfo[i].nWidth == 1632) &&
                            (pFormatList->FormatInfo[i].nHeight == 1224))
                        {
                            strFormat += " - 2M";
                        }
                        else if ((pFormatList->FormatInfo[i].nWidth == 1280) &&
                                 (pFormatList->FormatInfo[i].nHeight == 960))
                        {
                            strFormat += " - SXGA";
                        }
                    }

                    if ((pFormatList->FormatInfo[i].nWidth == 1280) &&
                        (pFormatList->FormatInfo[i].nHeight == 720))
                    {
                        strFormat += " - HD 16:9";
                    }
                    else if ((pFormatList->FormatInfo[i].nWidth == 800) &&
                        (pFormatList->FormatInfo[i].nHeight == 480))
                    {
                        strFormat += " - WVGA";
                    }
                    else if ((pFormatList->FormatInfo[i].nWidth == 640) &&
                        (pFormatList->FormatInfo[i].nHeight == 480))
                    {
                        strFormat += " - VGA";
                    }
                    else if ((pFormatList->FormatInfo[i].nWidth == 400) &&
                        (pFormatList->FormatInfo[i].nHeight == 240))
                    {
                        strFormat += " - WQVGA";
                    }
                    else if ((pFormatList->FormatInfo[i].nWidth == 352) &&
                        (pFormatList->FormatInfo[i].nHeight == 288))
                    {
                        strFormat += " - CIF";
                    }
                    else if ((pFormatList->FormatInfo[i].nWidth == 288) &&
                        (pFormatList->FormatInfo[i].nHeight == 352))
                    {
                        strFormat += " - CIF";
                    }
                    else if ((pFormatList->FormatInfo[i].nWidth == 320) &&
                        (pFormatList->FormatInfo[i].nHeight == 240))
                    {
                        strFormat += " - QVGA";
                    }
                    else if ((pFormatList->FormatInfo[i].nWidth == 240) &&
                        (pFormatList->FormatInfo[i].nHeight == 320))
                    {
                        strFormat += " - QVGA";
                    }

                    if ((pFormatList->FormatInfo[i].nSupportedCaptureModes & CAPTMODE_FREERUN) &&
                        (pFormatList->FormatInfo[i].nSupportedCaptureModes & CAPTMODE_SINGLE)
                       )
                    {
                        cB_FormatsLive->addItem(strFormat, pFormatList->FormatInfo[i].nFormatID);
                    }

                    if (pFormatList->FormatInfo[i].nSupportedCaptureModes & CAPTMODE_TRIGGER_SOFT_SINGLE)
                    {
                        cB_FormatsTrigger->addItem(strFormat, pFormatList->FormatInfo[i].nFormatID);
                    }
                }
            }
        }
        else
        {
            qDebug("error: is_ImageFormat returned %d", ret);
        }

        delete (pFormatList);
    }
    else
    {
        qDebug("error: is_ImageFormat returned %d", ret);
    }
    return ret;
}

void properties::selectComboImageFormats()
{
    int nFormatIx = 0;

    // find the current live format id in combobox data
    nFormatIx = cB_FormatsLive->findData(m_pDlgMainView->m_CameraProps.nImgFmtNormal);
    if (nFormatIx != -1)
    {
        cB_FormatsLive->setCurrentIndex(nFormatIx);
    }
    else
        qDebug("error: image format %d not found in live formats", m_pDlgMainView->m_CameraProps.nImgFmtNormal);

    // find the current trigger format id in combobox data
    nFormatIx = cB_FormatsTrigger->findData(m_pDlgMainView->m_CameraProps.nImgFmtTrigger);
    if (nFormatIx != -1)
    {
        cB_FormatsTrigger->setCurrentIndex(nFormatIx);
    }
    else
        qDebug("error: image format %d not found in trigger formats", m_pDlgMainView->m_CameraProps.nImgFmtTrigger);

}

void properties::changedFormatsLive (int index)
{
    int ret = 0;
    qDebug("image format ix LIVE %d choosen.", index);
    int nFormatId = cB_FormatsLive->itemData(index).toInt();

    if (m_bLive)
        is_StopLiveVideo (m_hCamera, IS_WAIT);

    if ((ret=is_ImageFormat(m_hCamera, IMGFRMT_CMD_SET_FORMAT, (void*)&nFormatId, sizeof(nFormatId))) == IS_SUCCESS)
    {
        m_pDlgMainView->m_CameraProps.nImgFmtNormal = nFormatId;
    }
    else
    {
        qDebug("error in on_cB_FormatsLive_currentIndexChanged: is_ImageFormat returned %d", ret);
    }

    m_pDlgMainView->SetupCapture();

    if (m_bLive)
        is_CaptureVideo (m_hCamera, IS_DONT_WAIT);

}

void properties::changedFormatsTrigger (int index)
{
    qDebug("image format ix TRIGGER %d choosen.", index);
    int nFormatId = cB_FormatsTrigger->itemData(index).toInt();
    m_pDlgMainView->m_CameraProps.nImgFmtTrigger = nFormatId;
}


void properties::OnMirrorUpDown ()
{
    BOOL bEnable = cB_Mirror_ud->isChecked ();
    is_SetRopEffect (m_hCamera, IS_SET_ROP_MIRROR_UPDOWN, bEnable, 0);
}

void properties::OnMirrorLeftRight ()
{
    BOOL bEnable = cB_Mirror_lr->isChecked ();
    is_SetRopEffect (m_hCamera, IS_SET_ROP_MIRROR_LEFTRIGHT, bEnable, 0);
}


