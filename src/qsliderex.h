/*
 * file    qsliderex.h
 * version 0.1
 *
 * extended QSlider class provides additional functionality
 *
 */

#ifndef QSLIDEREX_H_
#define QSLIDEREX_H_

#include <QObject>
#include <QSlider>
#include <QTimer>

/* Extended Slider class */
class QSliderEx:public QSlider
{
Q_OBJECT

public:
    /* Standard constructor */
    QSliderEx(QWidget *parent = 0, int slack = 0);
    /* Add constructor */
    QSliderEx(Qt::Orientation orientation, QWidget *parent = 0, int slack = 0);
    /* Destructor */
    virtual ~QSliderEx();
    /* Set slider slackness */
    void setSlackness(int slack);
    /* Base class slider signaling */
    void EnableSignaling(bool bEnable);

signals:
    /*
     * Slacked valueChange signal
     * value: new slider value
     */
    void valueChangedEx (int value); // slacked valueChanged

private slots:
    /* Slider init */
    void Init();

    /*
     * Base class value has changed slot
     * value: new base class value
     */
    void baseValueChanged(int value);
    /* Slack timeout slot */
    void slacktimeout();

private:
    /* Last slack emitted value */
    int m_lastvalue;
    /* Current base class value */
    int m_value;
    /* Signal slackness */
    int m_slack;
    /* Slack timer */
    QTimer *m_pTimer;
};

#endif
