/* [Tab/Indent: 8/8][Line/Box: 80/74]                    (CustomStatusBar.cpp) *
********************************************************************************

    Contents Description: Implementation of the CustomStatusBar class for a 
    sample program demonstrating an expanded use of the status bar within
    the Win32++ Windows framework, Copyright (c) 2005-2017 David Nash.

    This particular program was adapted from the StatusBar sample progam
    in the Win32++ distribution by Robert C. Tausworthe to illustrate the 
    definition of the status bar makeup by way of a data structure, in a
    similar fashion as that appearing in the Microsoft Foundation Classes 
    (MFC), and to override the Win32++ normal statusbar definition of four 
    parts to any number suitable for an application.

    The code changes to the StatusBar sample program only occur in the 
    CMainFrame and MyStatusBar classes. However, the entire program has
    been reformatted for better readability, per the Programming Notes,
    below.

    These materials are provided "as is", without warranty of any kind,
    express or implied, including but not limited to: warranties of
    merchantability, fitness for a particular purpose, and non-infringement.

    Usage:  The content of the custom status bar in an application is
    determined by a vector of PartInfo structs, in which the index of an
    entry sets the content, size and style of that status bar part. Each 
    PartInfo packet contains a numeric identifier, a size, a style, and an 
    internally used string value.  If the identifier is that of a resource 
    string or icon, then that string or icon is placed in the status bar part 
    associated with this struct. If a part is designated to contain one of 
    the CAP, NUM, or SCRL resources, the corresponding keyboard CAP, NUM, 
    or SCRL lock state is displayed when set. Other string resource 
    identifiers will also appear on the status bar. 
    
    PartInfo entries designated to have the SBT_OWNERDRAW style are owner 
    drawn by the OnMessageReflect() method.  In this demo, only one such
    owner drawn entry is accommodated.
    
    Other ewntries may be used for special purposes, such as the display 
    of progress contols or hypertext links. Such special usages should be 
    defined in the OnAttach() method and set to occupy their designated 
    parts in the PositionCustomParts() method.

    The size designation in each PartInfo struct is either an extent in
    pixel units, or else the SB_AUTOSIZE value. If a pixel size is given,
    that value prevails. If the SB_AUTOSIZE value appears and the identifier
    is that of a string or icon, the size is changed to fit the item(s).
    The remaining parts designated SB_AUTOSIZE share in the available client 
    width remaining after the fixed sizes have been allocated.

    Note: all string and icon resources inserted into parts of the status
    bar continue to appear until assignments to these parts are made in 
    other parts of the program and the status bar is redrawn.

    To use this class, insert into the CMainFrame class definition an
    object of this type,

        CustomStatusBar m_CustomStatusBar;

    Then, override the CFrame::GetStatusBar() method by placing the 
    following in the CMainFrame class declaration file:

        virtual CustomStatusBar& GetStatusBar() const
            {   return const_cast<CustomStatusBar&>
                (m_CustomStatusBar);
            }

    In the same file, override the CFrame::SetStatusIndicators() method
    by entering

        virtual void SetStatusIndicators();

    and defining this override as 

        void CMainFrame::SetStatusIndicators() 
        {
            m_CustomStatusBar.SetStatusIndicators();
        }

    The owner drawn text should be declared in the main frame by entering

        m_CustomStatusBar.SetOwnerDrawText(_T("Owner Draw"));

    or some other message appropriate to the application.

    Finally, define the parts of the custom status bar by entering 
    AddPartInfo(nID, size, style) statements in the class constructor.

    Programming Notes:
                The programming conventions used here roughly follow those 
        established by the 1997-1999 Jet Propulsion Laboratory Deep 
        Space Network Planning and Preparation Subsystem project for 
        C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include "CustomStatusBar.h"
#include "resource.h"
#include "App.h"

const UINT GRIPPER_SIZE = 20;
const UINT BORDER_SIZE  =  8;

/*============================================================================*/
    CustomStatusBar::
CustomStatusBar()                           /*
  
    Construct a CustomStatusBar object and populate the PartInfo array
    that defines the parts and content of the status bar.
*-----------------------------------------------------------------------------*/
{
    AddPartInfo(IDM_STATUS_MESSAGE, SB_AUTOSIZE, SBT_NOBORDERS); 
    AddPartInfo(IDW_MAIN,       SB_AUTOSIZE); 
    AddPartInfo(IDM_STATUS_HLINK,    80); 
    AddPartInfo(IDM_PROGRESS,   100);        
    AddPartInfo(IDM_BLANK,      100,         SBT_OWNERDRAW); 
    AddPartInfo(IDW_INDICATOR_CAPS, SB_AUTOSIZE, SBT_POPOUT);
    AddPartInfo(IDW_INDICATOR_NUM,  SB_AUTOSIZE, SBT_POPOUT);
    AddPartInfo(IDW_INDICATOR_SCRL, SB_AUTOSIZE, SBT_POPOUT);
}

/*============================================================================*/
    int CustomStatusBar::
IDtoPart(UINT nIDSeek)                          /*

    Return the part number of the numeric identifier nIDSeek in the 
    status bar PartInfo vector if it exists there; otherwise, return -1.
*-----------------------------------------------------------------------------*/
{
    int size = static_cast<UINT>(m_statusbar_part.size());
    for (int i = 0; i < size; i++)
        if (m_statusbar_part[i].ID == nIDSeek)
            return i;

    return -1;
}

/*============================================================================*/
    void CustomStatusBar::
OnAttach()                              /*

    Override the CStatusBar::OnAttach() method to define special status
    bar part objects.
*-----------------------------------------------------------------------------*/
{
      // Assign a numeric identifier to the status bar. This is needed 
      // in order for  the owner drawn pane to work)
    SetWindowLongPtr(GWLP_ID, IDM_STATUSBAR);
      // Start a timer for the progress bar
    SetTimer(101, 100, 0);
       // Create the Hyperlink
    m_Hyperlink.Create(*this);
      // Create the ProgressBar
    m_ProgressBar.Create(*this);
      // Set the background color 
    SendMessage(SB_SETBKCOLOR, 0, SBBkgrColor);
}

/*============================================================================*/
    BOOL CustomStatusBar::
OnEraseBkgnd(CDC& dc)                           /*

    Fill the background with a color gradient when XP themes are enabled.
*-----------------------------------------------------------------------------*/
{
    if (IsXPThemed())
    {
        dc.GradientFill(SBEraseColor[0], SBEraseColor[1], 
            GetClientRect(), TRUE); 
        return TRUE;
    }
    return FALSE;
}

/*============================================================================*/
    LRESULT CustomStatusBar::
OnMessageReflect(UINT uMsg, WPARAM wParam, LPARAM lParam)       /*

    Perform owner drawing of the status bar part designated as a drawing
    item by the lParam parameter.
*-----------------------------------------------------------------------------*/
{
    UNREFERENCED_PARAMETER(wParam);

    if (uMsg ==  WM_DRAWITEM)
    {      // it's a message from the owner draw pane of the status bar
        LPDRAWITEMSTRUCT pDrawItem = (LPDRAWITEMSTRUCT) lParam;
        CRect rcPart = pDrawItem->rcItem;
        CDC dc;
        dc.Attach(pDrawItem->hDC);
          // Set the font to italic
        CFont Font = GetFont();
        LOGFONT lf = Font.GetLogFont();
        lf.lfItalic = TRUE;
        dc.CreateFontIndirect(lf);
          // Display the gradient background and text
        dc.GradientFill(ODBkgrColor[0], ODBkgrColor[1], 
            rcPart, TRUE);
        dc.SetTextColor(ODTextColor);
        dc.SetBkMode(TRANSPARENT);
        dc.TextOut(rcPart.left, rcPart.top, m_ODMessage,
            m_ODMessage.GetLength());
          // release the attachment above
        dc.Detach();
          // indicate that the message was handled
        return TRUE;
    }
    return 0L;
}

/*============================================================================*/
    void CustomStatusBar::
PositionCustomParts()                           /*

    Reposition custom objects to their designated status bar parts when
    the status bar is resized.
*-----------------------------------------------------------------------------*/
{
      // set the progress control into its part
    int part = IDtoPart(IDM_PROGRESS);
    if (part >= 0)
    {
        CRect rcPart = GetPartRect(part);
        m_ProgressBar.SetWindowPos(NULL, rcPart, SWP_SHOWWINDOW);
    }
      // set the hyperlink  into its part
    part = IDtoPart(IDM_STATUS_HLINK);
    if (part >= 0)
    {
        CRect rcPart = GetPartRect(part);
        m_Hyperlink.SetWindowPos(NULL, rcPart, SWP_SHOWWINDOW);
    }
}

/*============================================================================*/
    void CustomStatusBar::
PreCreate(CREATESTRUCT& cs)                     /*

*-----------------------------------------------------------------------------*/
{
    cs.style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | CCS_BOTTOM | 
        WS_CLIPCHILDREN;  // to reduce flicker on resizing

      // Suppress the gripper unless XP themes are enabled
    if (IsXPThemed())
    {
        cs.style |= SBARS_SIZEGRIP;
    }
}

/*============================================================================*/
    void CustomStatusBar::
SetStatusIndicators()                           /*

    Place string, icon, and custom resources into their designated status 
    bar parts. Designate owner drawn parts to be filled by the main frame 
    OnDraw() method. Recompute the sizes of parts, as these may have 
    changed due to resizing.
*-----------------------------------------------------------------------------*/
{
    UINT nIDCount = static_cast<UINT>(m_statusbar_part.size());
    if (!IsWindow() || nIDCount == 0)
        return;

      // load panes with specified nID strings, count the number of 
      // SB_AUTOSIZE's, and calculate committed width
    UINT nAutoWidths = 0;
      // make room for a gripper control if the style says it's there
    UINT cxGripper = (GetStyle() & SBARS_SIZEGRIP) != 0 ?
        GRIPPER_SIZE : 0;
    UINT cumulativeWidths = cxGripper; // space for gripper, if present
    for (UINT i = 0; i < nIDCount; i++)
    {
        PartInfo part = m_statusbar_part[i];
        UINT nID = part.ID;
          // set the pane size for icons and strings
        CString s = LoadString(nID);
        HICON h = (HICON)GetApp()->LoadImage(nID, IMAGE_ICON,
            16, 16, LR_DEFAULTCOLOR | LR_SHARED);
        if (h != NULL && part.width == SB_AUTOSIZE)
        {     // icon widths are square, so use height
            UINT width = GetPartRect(0).Height();
            m_statusbar_part[i].width = width + BORDER_SIZE;
        }
        if (!s.IsEmpty() && part.width == SB_AUTOSIZE)
        {     // get a device context with the correct font
            CClientDC dcStatus(*this);
            dcStatus.SelectObject(GetFont());
            // get the pane width to fit the text, logical units
            CSize size = dcStatus.GetTextExtentPoint32(s, 
                s.GetLength());
              // convert size to device units
            CRect rc(0, 0, size.cx, size.cy);
            dcStatus.LPtoDP(rc);
            m_statusbar_part[i].width +=  rc.Width() + BORDER_SIZE;
        }
          // count remaining auto-sized parts and committed width
        part.width = m_statusbar_part[i].width;
        if (part.width == SB_AUTOSIZE)
            nAutoWidths++;
        else
            cumulativeWidths += part.width;
    }
      // distribute available width among auto_sized panes
    int auto_size = nAutoWidths > 0 ? 
        (GetClientRect().Width() - cumulativeWidths) / nAutoWidths : 0;
      // set part, icons, and owner drawn parts         
    for (UINT j = 0; j < nIDCount; j++)
    {
        BOOL skip = FALSE; // skip if sStatus is not of use
        PartInfo part = m_statusbar_part[j];
        UINT nID = part.ID;
          // if this nID has an icon, set in the part, but only if
          // different than the current icon.
        HICON hPart = GetPartIcon(j),
              h     = (HICON)GetApp()->LoadImage(nID, IMAGE_ICON,
                    16, 16, LR_DEFAULTCOLOR | LR_SHARED);
        if (h != NULL && hPart != h)
            SetPartIcon(j, h);
          // evaluate strings and owner drawn
        CString sStatus, 
            idString = LoadString(nID);
        switch (nID)
        {
            case IDW_INDICATOR_CAPS:
            sStatus = (::GetKeyState(VK_CAPITAL) & 0x0001) ?  
                idString : CString(_T(" "));
            break;

            case IDW_INDICATOR_NUM:
            sStatus = (::GetKeyState(VK_NUMLOCK) & 0x0001)? 
                idString : CString(_T(" "));
            break;

            case IDW_INDICATOR_SCRL:
            sStatus = (::GetKeyState(VK_SCROLL) & 0x0001)? 
                idString : CString(_T(" "));
            break;

            default:
            if (part.style & SBT_OWNERDRAW)
            {    // Set this part as owner drawn. The drawing  
                 // is performed in OnMessageReflect().
                sStatus = m_ODMessage;
            }
            else
            {     // if the part nID has a resource string, 
                   //use it; otherwise, do not alter this entry
                sStatus = idString;
                skip = idString.IsEmpty();
            }
            break;
        }
          // reset the size in case the width changed
        int size = (part.width == SB_AUTOSIZE ? 
            auto_size : m_statusbar_part[j].width);
        SetPartWidth(j, size);
          // reset indicators only if they changed
        if (!skip && sStatus != m_statusbar_part[j].old)
        {
            SetPartText(j, sStatus, part.style);
            m_statusbar_part[j].old = sStatus;
        }
    }
      // reset parts requiring relocation
    PositionCustomParts();
}

/*============================================================================*/
    LRESULT CustomStatusBar::
WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)            /*

*-----------------------------------------------------------------------------*/
{

    switch(uMsg)
    {
        case WM_TIMER:
        {   
        // Change the Progress Bar indication
        m_ProgressBar.OffsetPos(1);
        if (m_ProgressBar.GetRange(FALSE) == m_ProgressBar.GetPos())
            m_ProgressBar.SetPos(0);

        break;
        }
    }
    return WndProcDefault(uMsg, wParam, lParam);
} 
