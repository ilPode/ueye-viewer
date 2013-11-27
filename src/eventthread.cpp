/*
 * file    eventthread.cpp
 * version 0.1
 *
 * eventthread class (inherits QThread) implementation
 * to wait on a uEye event and signal a connected slot
 *
 */

#include "eventthread.h"

EventThread::EventThread ()
{
    m_hCam = 0;
    m_event = 0;
    m_bRunEventThread = false;
#ifndef __LINUX__
    m_hEvent = 0;
#endif
}

EventThread::~EventThread ()
{

}

int EventThread::start (HIDS hCam,int event)
{
    int ret = 0;
    m_hCam = hCam;
    m_event = event;
#ifndef __LINUX__
    ret = is_InitEvent(m_hCam, m_hEvent, m_event);
#endif
    ret = is_EnableEvent (m_hCam, m_event);

    if (ret == 0)
    {
        //printf ("started event %d detection!\n", m_event);
        m_bRunEventThread = true;
        QThread::start (); // start the thread run function
    }

    return ret;
}

void EventThread::stop ()
{
    m_bRunEventThread = false;
}

void EventThread::run ()
{
    //printf ("start waiting on event %d\n", m_event);
    while (m_bRunEventThread)
    {
#ifdef __LINUX__
        if (is_WaitEvent (m_hCam, m_event, 1) == IS_SUCCESS)
#else
        if (WaitForSingleObject(m_hEvent, 1000) == WAIT_OBJECT_0)
#endif
        {
            //printf ("event %d detected\n", m_event);
            emit eventhappen (m_event);
        }
    }
    m_bRunEventThread = FALSE;
    is_DisableEvent (m_hCam, m_event);
#ifndef __LINUX__
    is_ExitEvent(m_hCam, m_event);
#endif
    //printf ("stop waiting on event %d\n", m_event);
}

