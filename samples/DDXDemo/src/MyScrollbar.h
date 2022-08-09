/* (24-Jul-2015) [Tab/Indent: 8/8][Line/Box: 80/74]            (MyMyScrollbar.h) *
********************************************************************************
|                                                                              |
|                      Author: Robert C. Tausworthe, 2020                      |
|                                                                              |
===============================================================================*

    Contents Description: The custom MyScroll bar class declaration for
    this DDX-DDV Test program. This class is derived from the MyScrollBar
    class found in the Win32++ Windows sample programs.

    Programming Notes: The programming style roughly follows that established
    by the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

    Acknowledgement: The author would like to thank and acknowledge the advice,
    critical review, insight, and assistance provided by David Nash in the
    development of this work.


*******************************************************************************/

#ifndef MYSCROLLBAR_H
#define MYSCROLLBAR_H

/*============================================================================*/
    class
CMyScrollBar : public CScrollBar                                         /*

*-----------------------------------------------------------------------------*/
{
    public:
        CMyScrollBar();
        virtual ~CMyScrollBar() {}

        virtual int     GetHScrollPos(UINT, WPARAM, LPARAM);
                void    SetScrollInfo(int lo, int hi, int pos, int page);

    private:
        CMyScrollBar(const CMyScrollBar&);                // Disable copy construction
        CMyScrollBar& operator = (const CMyScrollBar&);   // Disable assignment operator

        SCROLLINFO m_scrollInfo;
};

/*----------------------------------------------------------------------------*/
#endif  // MYSCROLLBAR_H
