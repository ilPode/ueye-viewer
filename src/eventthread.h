/*
 * file    eventthread.h
 * version 0.1
 *
 * eventthread class (inherits QThread) declaration
 * to wait on a uEye event and signal a connected slot
 *
 */

#ifndef EVENTTHREAD_H_
#define EVENTTHREAD_H_

#include <QObject>
#include <QThread>
#include <uEye.h>


/*
 * Event thread implementation for camera list change handling
 *
 * eventthread class inherits QThread to use signal/slot mechanism to signal an
 * initialized uEye event to a connected slot. For each uEye event to wait for, should
 * be a seperate member of this eventthread class.
 * 
 * To use signal/slot mechanism no multiple inheritance from an Qt Object class
 * is allowed.
 */
class EventThread:public QThread
{
Q_OBJECT

public:
    /* Constructor and destructor */
    EventThread ();
    virtual ~EventThread ();

    /*
     * Starts waiting on a uEye event
     * hCam: uEye camera handle
     * event: waiting for
     */
    int start (HIDS hCam = 0,int event = 0);
    
    /* Stops waiting on a uEye event */
    void stop ();

signals:
    /* Qt signal function to be emited, when the uEye event happens */
    void eventhappen (int event);

protected:
    /* EventThread run function. Waits for the uEye event. */
    void run ();

private:
    /* camera handle for waiting event */
    HIDS m_hCam;
    /* event waiting for */
    int m_event;
    /* event run switch */
    volatile bool m_bRunEventThread;
#ifndef __LINUX__
    /* on windows we need an Event handle member */
    HANDLE m_hEvent;
#endif

};

#endif 
