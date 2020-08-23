/* (24-Jul-2015) [Tab/Indent: 8/8][Line/Box: 80/74]              (MyDateTime.h) *
********************************************************************************
|                                                                              |
|                      Author: Robert C. Tausworthe, 2020                      |
|                                                                              |
===============================================================================*

    Contents Description: The CMyDateTime class implementation. This
    class is derived from the CDateTime class found in the Win32++
    Windows interface classes. It provides ancillary functions useful
    to the current applications.

    Programming Notes: The programming standards roughly follow those
    established by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
    Planning and Preparation Subsystem project for C++ programming.

    Acknowledgement: The author would like to thank and acknowledge the advice,
    critical review, insight, and assistance provided by David Nash in the
    development of this work.

*******************************************************************************/

#ifndef MyDateTime_H
#define MyDateTime_H

/*============================================================================*/
    class
CMyDateTime : public CDateTime                                           /*

    This class extends the CDateTime class to get the selected date-time
    entry in the form of a CString.
*-----------------------------------------------------------------------------*/
{
    public:
        CMyDateTime() {}
        virtual ~CMyDateTime() {}

//        CString GetTimeStr(const CString&) const;     TBD: UNUSED

    protected:

    private:

};
/*----------------------------------------------------------------------------*/
#endif  // MyDateTime_H
