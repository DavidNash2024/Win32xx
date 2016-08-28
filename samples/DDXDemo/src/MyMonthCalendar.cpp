/* (24-Jul-2015) [Tab/Indent: 8/8][Line/Box: 80/74]      (MyMonthCalendar.cpp) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2015, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                                                                              |
===============================================================================*

	Contents Description: The CMyMonthCalendar class implementation. This
	class is derived from the CMonthCalendar class found in the Win32++
	Windows interface classes, Copyright (c)2005-2015 David Nash, under
	permissions granted therein. It cleans up the SYSTEMTIME interface
	with the monthly calendar control.

        Caveats: The copyright displayed above extends only to the author's
	original contributions to the subject class, and  to the alterations,
	additions, deletions, and  other treatments of materials that may have
	been extracted from the cited sources.  Unaltered portions of those
	materials retain their original copyright status. The author hereby
	grants permission to any person obtaining a copy of this treatment
	of the subject class and  any associated documentation composed by
	the author, to utilize this material, free of charge and  without
	restriction or limitation, subject to the following conditions:

		The above copyright notice, as well as that of David Nash
		and Win32++, together with the respective permission
		conditions shall be included in all copies or substantial
		portions of this material so copied, modified, merged,
		published, distributed, or otherwise held by others.

	These materials are provided "as is", without warranty of any kind,
	express or implied, including but not limited to: warranties of
	merchantability, fitness for a particular purpose, and non-infringement.
	In no event shall the authors or copyright holders be liable for any
	claim, damages, or other liability, whether in an action of contract,
	tort or otherwise, arising from, out of, or in connection with, these
	materials, the use thereof, or any other other dealings therewith.

	Special Conventions:

	Programming Notes:
                The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
		Planning and Preparation Subsystem project for C++ programming.
		
	Acknowledgement:
	The author would like to thank and acknowledge the advice, critical
	review, insight, and assistance provided by David Nash in the development
	of this work.		

********************************************************************************

	Implementation of the CMyMonthCalendar class

*******************************************************************************/

#include "stdafx.h"
#include "MyMonthCalendar.h"

/*============================================================================*/
SYSTEMTIME CMyMonthCalendar::
GetCurSel() const     					/*

	Retrieve the currently selected date, with hour, minute, seconds, and
	milliseconds fields set to zero.  The base class values of these
	fields are seemingly random numbers that distort the true date in
	the control.
*-----------------------------------------------------------------------------*/
{
	SYSTEMTIME st = CMonthCalendar::GetCurSel();
	st.wHour = st.wMinute = st.wSecond = st.wMilliseconds = 0;
	return st;
}

/*============================================================================*/
	CString CMyMonthCalendar::
GetCurSelStr(const CString &fmt) const     					/*

	Return the currently selected date as a CString.
*-----------------------------------------------------------------------------*/
{
	CTime t(GetCurSel());
	CString mc = t.Format(fmt);

	return mc;
}

