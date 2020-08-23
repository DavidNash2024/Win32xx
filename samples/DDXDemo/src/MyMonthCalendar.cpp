/* (24-Jul-2015) [Tab/Indent: 8/8][Line/Box: 80/74]      (MyMonthCalendar.cpp) *
********************************************************************************
|                                                                              |
|                      Author: Robert C. Tausworthe, 2020                      |
|                                                                              |
===============================================================================*

    Contents Description: The CMyMonthCalendar class implementation. This
    class is derived from the CMonthCalendar class found in the Win32++
    Windows interface classes. It cleans up the SYSTEMTIME interface
    with the monthly calendar control.

    Programming Notes: The programming standards roughly follow those
    established by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
    Planning and Preparation Subsystem project for C++ programming.

    Acknowledgement: The author would like to thank and acknowledge the advice,
    critical review, insight, and assistance provided by David Nash in the
    development of this work.

*******************************************************************************/

#include "stdafx.h"
#include "MyMonthCalendar.h"

/*============================================================================*/
//SYSTEMTIME CMyMonthCalendar::
//GetCurSel() const                                                           /*
/*
    Retrieve the currently selected date, with hour, minute, seconds, and
    milliseconds fields set to zero.  The base class values of these
    fields are seemingly random numbers that distort the true date in
    the control.
*-----------------------------------------------------------------------------*/
//{
//    SYSTEMTIME st = CMonthCalendar::GetCurSel();
//    st.wHour = st.wMinute = st.wSecond = st.wMilliseconds = 0;
//    return st;
//}

/*============================================================================*/
//    CString CMyMonthCalendar::
//GetCurSelStr(const CString &fmt) const                                      /*
/*
    Return the currently selected date as a CString.
*-----------------------------------------------------------------------------*/
//{
//    CTime t(GetCurSel());
//    CString mc = t.Format(fmt);
//
//    return mc;
//}
/*----------------------------------------------------------------------------*/
