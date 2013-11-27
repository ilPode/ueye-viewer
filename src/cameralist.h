/*
 * file     cameralist.h
 * version  0.1
 *
 * Camera list widget declaration
 *
 */

#ifndef CAMERALIST_H
#define CAMERALIST_H

#include <QDialog>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <uEye.h>

/* Number of camera list columns */
#define TABLE_COLUMNS 4

/*
 * Camera list widget control
 */
class CameraList : public QDialog
{
Q_OBJECT

public:
    /* Constructor and destructor */
    CameraList (QWidget *parent = 0);
    ~CameraList ();

    /* Get number of cameras */
    unsigned int Size ();

    /*
     * Select a camera from the camera list
     * iCam: Camera index
     * CamID: CameraID
     */
    int SelectCamera (unsigned int iCam);
    int SelectCameraByID (unsigned int CamID);
    
    UEYE_CAMERA_INFO GetCameraInfo ()
    {
        return m_CameraInfo;
    }

public slots:
    void OnClose ();
    void Update ();

private:
    /* Initialize camera list */
    bool InitCameraList ();

    /* Delete camera list */
    void DeleteCameraList ();
    
    /* Build camera information table */
    void BuildTable ();

    /*
     * Add table row
     * items: Number of items
     */
    void addRow (int items);

    /* Clear the table */
    void clearTable ();

    /* Camera list array */
    PUEYE_CAMERA_LIST m_pCamList;

    /* Current camera info */
    UEYE_CAMERA_INFO m_CameraInfo;

    /* Qt table widget for camera list display */
    QTableWidget* m_pTableWidget;

    /* Qt button widget */
    QPushButton* m_pBut;

    /* Qt box layout widget */
    QVBoxLayout *m_pLayout;

};

#endif 
