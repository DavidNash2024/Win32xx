/* (28-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]            (PrintInfo.cpp) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Implementation of the CPrintInfo class for
    applications using the Win32++ Windows interface classes. The class
    is described further below.

    About the CPrintInfo class: CPrintInfo stores information about a print
    or print-preview job. The CPrintView class creates a CPrintInfo object
    each time the Print or Print Preview command is chosen and destroys it
    once the printing task is completed.

    During the CPrintView cycle, the CPrintInfo object contains persistent
    information about the print job as a whole, such as the range of pages
    to be printed and the current status of the print job, including the
    page currently being printed. Some of this information is obtained from,
    and given into, an associated CPrintDialog object; the CPrintInfo object
    retains the values entered by the user in the Print dialog box. This
    information remains as the CPrintDialog object goes out of scope. (This
    is different than the MFC version of this class).

    A CPrintInfo object is passed between the base CPrintView class and its
    derived class during the printing process and is used to exchange
    information between the two. For example, the base class informs the
    derived class which page of the document to print by assigning a value
    to the m_nCurPage member of CPrintInfo; the derived class retrieves this
    value and performs the actual printing of the specified page.

    Another example is the case in which the length of the document is not
    known until it is printed. In this situation, the derived class may
    test for the end of the document each time a page is printed. When the
    end is reached, the derived class may set the m_bContinuePrinting member
    of CPrintInfo to FALSE; this informs the base class to stop the print
    loop.

    Programming Notes: The programming style roughly follows that established
    by the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include "PrintInfo.h"

/*============================================================================*/
    CPrintInfo::
CPrintInfo()                                                            /*

*-----------------------------------------------------------------------------*/
{
    m_pPD = NULL;
    m_bPreview   = FALSE;       // initialize to not preview
    m_bDirect    = FALSE;       // initialize to not direct
    m_bDocObject = FALSE;       // initialize to not IPrint
    m_bContinuePrinting = FALSE;    // Assume it is not yet OK to print

    m_dwFlags = 0;                  // not supported
    m_nOffsetPage = 0;              // not supported

    m_nCopies    = 1;       // number of copies to print
    m_nFromPage  = 1;       // first printed page, 1 based
    m_nMaxPage   = 0xffff;      // maximum page limit
    m_nMinPage   = 1;       // minimum page limit
    m_nToPage    = 0xffff;      // last printed page
    m_nCurPage   = 0;       // Current page

    m_lpUserData = NULL;        // pointer to user created struct
    m_nNumPreviewPages = 0;     // (not supported)
    m_rectDraw.SetRect(0, 0, 0, 0); // rect of current usable page area
    m_strPageDesc = _T("Page %u");  // Format string for page number display
    m_nMargin     = 0;     // page margin, in twips
}

/*============================================================================*/
    void CPrintInfo::
InitInfo(CPrintDialog* pPD, UINT fromPage, UINT toPage, UINT minPage,
    UINT maxPage, UINT nCopies)                     /*

        Set initial values for the CPrintInfo object. This member must be
    invoked with a valid CPrintDialog pPD before a DoModal operation using
    the object for other uses.
*-----------------------------------------------------------------------------*/
{
    assert(pPD);
    m_pPD        = pPD;
    PRINTDLG pd  = m_pPD->GetParameters();
    pd.nCopies   = (WORD)nCopies;
    pd.nFromPage = (WORD)fromPage;
    pd.nToPage   = (WORD)toPage;
    pd.nMinPage  = (WORD)minPage;
    pd.nMaxPage  = (WORD)maxPage;
    m_pPD->SetParameters(pd);
}
/*----------------------------------------------------------------------------*/
