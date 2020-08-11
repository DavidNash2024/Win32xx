/* (24-Jul-2015) [Tab/Indent: 8/8][Line/Box: 80/74]            (MyMyScrollbar.h) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2015, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                                                                              |
===============================================================================*

    Contents Description: The custom MyScroll bar class declaration for
    this DDX-DDV Test program. This class is derived from the MyScrollBar
    class found in the Win32++ Windows sample programs, Copyright
    (c)2005-2015 David Nash, under permissions granted therein.

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

    Declaration of the CMyScrollBar class

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

        virtual LRESULT OnHScroll(UINT, WPARAM, LPARAM);
        virtual LRESULT OnMessageReflect(UINT, WPARAM, LPARAM);
        void    SetScrollInfo(int lo, int hi, int pos, int page);

    private:
        SCROLLINFO m_siScroll;
};

/*----------------------------------------------------------------------------*/
#endif  // MYSCROLLBAR_H
