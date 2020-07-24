/* (14-Nov-2016) [Tab/Indent: 4/4][Line/Box: 80/74]                   (View.h) *
********************************************************************************
|                                                                              |
|                         robert.c.tausworthe@ieee.org                         |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of the CView class derived from the 
    CScrollView base class for this application using the Win32++ Windows 
    framework, Copyright (c) 2005-2017 David Nash, under permissions granted
    therein.

    The above copyright notice, as well as that of David Nash and Win32++, 
    together with the respective permissionconditions shall be included in all 
    copies or substantial portions of this material so copied, modified, merged,
    published, distributed, or otherwise held by others.

    These materials are provided "as is", without warranty of any kind,
    express or implied, including but not limited to: warranties of
    merchantability, fitness for a particular purpose, and non-infringement.
    In no event shall the authors or copyright holders be liable for any
    claim, damages, or other liability, whether in an action of contract,
    tort or otherwise, arising from, out of, or in connection with, these
    materials, the use thereof, or any other other dealings therewith.

    Programming Notes: The programming standards roughly follow those 
    established by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
    Planning and Preparation Subsystem project for C++ programming.

*******************************************************************************/

#ifndef CAPP_VIEW_H
#define CAPP_VIEW_H

#include "Doc.h"

/*============================================================================*/
    class
CView   : public CScrollView                                            /*

    The application scrolling window serializable class.
*-----------------------------------------------------------------------------*/
{
public:
    CView();
    virtual ~CView(){}

    // Attributes
    COLORREF  GetBkgndColor() const      { return m_bkgndColor; }
    COLORREF* GetColorsArray()           { return m_colors; }
    COLORREF  GetTextColor()  const      { return m_textColor; }
    CFont     GetTextFont()   const      { return m_font; }
    void      SetBkgndColor(COLORREF bk) { m_bkgndColor = bk; }
    void      SetTextFont(CFont font)    { m_font = font; }
    void      SetTextColor(COLORREF col) { m_textColor = col; }
    CDoc&     TheDoc()                   { return m_doc; }

    // Operations
    CSize   GetFontSize() const;
    void    SetAppSize(BOOL keepPos = FALSE);
    void    SetDefaultFont();
    void    SetDefaults();
    void    TextLineOut(CDC&, UINT, UINT, LPCTSTR) const;

    // public data members
    static const CString m_compiledOn;  // compilation date 

protected:
    virtual void    OnDraw(CDC& dc);
    virtual LRESULT OnPaint(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual void    PreCreate(CREATESTRUCT &cs);
    virtual void    Serialize(CArchive &ar);
    virtual LRESULT WndProc(UINT msg, WPARAM, LPARAM);

private:            
    // data members
    COLORREF    m_bkgndColor;   // background color
    CDoc        m_doc;          // the document       
    CFont       m_font;         // the font used to display view 
    COLORREF    m_colors[16];   // custom colors defined
    COLORREF    m_textColor;    // text foreground color
};
/*----------------------------------------------------------------------------*/
#endif  //CAPP_VIEW_H
