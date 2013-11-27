/*
 * file     imageinfodlg.h
 * version  0.1
 *
 * Additional image information dialog
 *
 */

#ifndef IMAGEINFODLG_H
#define IMAGEINFODLG_H

#include <QDialog>
#include "ui_imageinfodlg.h"
#include <uEye.h>

class Mainview;

/* Imageinfo dialog class declaration */
class imageinfodlg : public QDialog
{
    Q_OBJECT

public:
    /* Constructor and destructor */
    imageinfodlg(QWidget *parent = 0);
    ~imageinfodlg();
    /* Clears the dialog image info controls */
    void ClearControls ();

public slots:
    /* Resets the dialogs counter control values */
    void ResetCounters();
    /* Updates the dialogs counter control with current values */
    void UpdateCounters();
    /* 
     * Updates the dialogs image parameter controls
     * pImageinfo: pointer to a imageinfo struct 
     * */
    void UpdateControls(UEYEIMAGEINFO *pImageinfo);

signals:
    /* Close signal for the parent */
    void close();

protected:
    /* Overloads parents 'closeEvent' and 'showEvent' functions
     * e: pointer to Qt close event */
    void closeEvent (QCloseEvent * e);
    void showEvent (QShowEvent * e);

private:
    /* Dialogs GUI member class, build by Qt */
    Ui::imageinfodlgClass ui;
    /* MainView parent ref */
    Mainview* m_pParent;
};

#endif
