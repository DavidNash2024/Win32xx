/* [Tab/Indent: 8/8][Line/Box: 80/74]                    (CustomStatusBar.cpp) *
********************************************************************************

    Contents Description: Implementation of the CustomStatusBar class for a
    sample program demonstrating an expanded use of the status bar within
    the Win32++ Windows framework.

    Usage:  The content of the custom status bar in an application is
    determined by a vector of PartInfo structs, each of which sets the
    position, content, size and style of that status bar part. Each
    PartInfo packet contains a numeric identifier, a size, a style, and an
    internally used string value.  If the identifier is that of a resource
    string or icon, then that string or icon is placed in the status bar part
    associated with this struct. If a part is designated to contain one of
    the INS, CAP, NUM, or SCRL resources, the corresponding keyboard INS, CAP,
    NUM, or SCRL lock state is displayed when set. Other string resource
    identifiers will also appear on the status bar.

    Note: all string and icon resources inserted into parts of the status
    bar continue to appear until assignments to these parts are made in
    other parts of the program and the status bar is redrawn.

    The size designation in each PartInfo struct is either an extent in
    pixel units, or else the SB_AUTOSIZE value. If a pixel size is given,
    that value prevails. If the SB_AUTOSIZE value appears and the identifier
    is that of a string or icon, the size is changed to fit the item(s).
    The remaining parts designated SB_AUTOSIZE share in the available client
    width remaining after the fixed sizes have been allocated.

    The style refers to the third parameter of the CStatusBar::SetPartText()
    method, which sets the appearance of the statusbar. See the documentation
    of that method for particulars.

    PartInfo entries designated to have the SBT_OWNERDRAW style are owner
    drawn by the OnMessageReflect() method.  In this demo, only one such
    owner drawn entry is accommodated.

    Other entries may be used for special purposes, such as the display
    of progress controls or hypertext links. Such special usages should be
    defined in the OnAttach() method and set to occupy their designated
    parts in the PositionCustomParts() method.

    To use this class, insert into the CMainFrame class definition an
    object of this type,

        CustomStatusBar m_customStatusBar;

    Then, override the CFrame::GetStatusBar() method by placing the
    following in the CMainFrame class declaration file:

        virtual CustomStatusBar& GetStatusBar() const
            {   return const_cast<CustomStatusBar&>
                (m_customStatusBar);
            }

    In the same file, override the CFrame::SetStatusIndicators() method
    by entering

        virtual void SetStatusIndicators();

    and defining this override as

        void CMainFrame::SetStatusIndicators()
        {
            m_customStatusBar.SetStatusIndicators();
        }

    The owner drawn text should be declared in the main frame by entering

        m_customStatusBar.SetOwnerDrawText(_T("Owner Draw"));

    or some other message appropriate to the application.

    Finally, define the parts of the custom status bar by entering
    AddPartInfo(id, size, style) statements in the class constructor.

    Programming Notes: The programming conventions used here roughly follow
    those established by the 1997-1999 Jet Propulsion Laboratory Deep Space
    Network Planning and Preparation Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include "CustomStatusBar.h"
#include "resource.h"
#include "App.h"

const UINT GRIPPER_SIZE = 20;
const UINT BORDER_SIZE  =  8;

/*============================================================================*/
    CustomStatusBar::
CustomStatusBar()                                                           /*

    Construct a CustomStatusBar object and populate the PartInfo array
    that defines the parts and content of the status bar.
*-----------------------------------------------------------------------------*/
{
    AddPartInfo(IDM_STATUS_MESSAGE, SB_AUTOSIZE, SBT_NOBORDERS);
    AddPartInfo(IDW_MAIN,           SB_AUTOSIZE);
    AddPartInfo(IDM_STATUS_HLINK,    80);
    AddPartInfo(IDM_PROGRESS,       100);
    AddPartInfo(IDM_BLANK,          100,         SBT_OWNERDRAW);
    AddPartInfo(IDW_INDICATOR_CAPS, SB_AUTOSIZE, SBT_POPOUT);
    AddPartInfo(IDW_INDICATOR_NUM,  SB_AUTOSIZE, SBT_POPOUT);
    AddPartInfo(IDM_INDICATOR_INS,  SB_AUTOSIZE, SBT_POPOUT);
    AddPartInfo(IDW_INDICATOR_SCRL, SB_AUTOSIZE, SBT_POPOUT);
}

/*============================================================================*/
        void  CustomStatusBar::
AddPartInfo(UINT id, int size, UINT style /* = 0 */)                        /*

    Set the the next status bar part in sequence to have the given resource
    id, size, and style.
*-----------------------------------------------------------------------------*/
{
    PartInfo pi(id, size, style);
    m_statusBarParts.push_back(pi);
}

/*============================================================================*/
    int CustomStatusBar::
IDtoPart(UINT idSeek)                                                       /*

    Return the part number of the numeric identifier idSeek in the
    status bar PartInfo vector if it exists there; otherwise, return -1.
*-----------------------------------------------------------------------------*/
{
    int size = static_cast<UINT>(m_statusBarParts.size());
    for (int i = 0; i < size; ++i)
        if (m_statusBarParts[i].ID == idSeek)
            return i;

    return -1;
}

/*============================================================================*/
    void CustomStatusBar::
OnAttach()                                                                  /*

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
    m_hyperlink.Create(*this);
      // Create the ProgressBar
    m_progressBar.Create(*this);
      // Set the background color
    SendMessage(SB_SETBKCOLOR, 0, SBBkgrColor);
}

/*============================================================================*/
    BOOL CustomStatusBar::
OnEraseBkgnd(CDC& dc)                                                       /*

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
OnMessageReflect(UINT msg, WPARAM wparam, LPARAM lparam)                   /*

    Reflect the WM_OWNERDRAW message back here to perform owner drawing of
    that status bar part designated as a drawing item.
*-----------------------------------------------------------------------------*/
{
    UNREFERENCED_PARAMETER(wparam);

    if (msg ==  WM_DRAWITEM)
    {     // it's a message from the owner draw pane of the status bar
          // found in lparam
        LPDRAWITEMSTRUCT drawItem = reinterpret_cast<LPDRAWITEMSTRUCT>(lparam);
        CRect rcPart = drawItem->rcItem;
        CDC dc;
        dc.Attach(drawItem->hDC);
          // Set the font to italic
        CFont font = GetFont();
        LOGFONT lf = font.GetLogFont();
        lf.lfItalic = TRUE;
        dc.CreateFontIndirect(lf);
          // Display the gradient background and text
        dc.GradientFill(ODBkgrColor[0], ODBkgrColor[1],
            rcPart, TRUE);
        dc.SetTextColor(ODTextColor);
        dc.SetBkMode(TRANSPARENT);
        dc.TextOut(rcPart.left, rcPart.top, m_ownerDrawMsg,
            m_ownerDrawMsg.GetLength());
          // release the attachment above
        dc.Detach();
          // indicate that the message was handled
        return TRUE;
    }
    return 0L;
}

/*============================================================================*/
    void CustomStatusBar::
PositionCustomParts()                                                       /*

    Reposition custom objects to their designated status bar parts when
    the status bar is resized.
*-----------------------------------------------------------------------------*/
{
      // set the progress control into its part
    int part = IDtoPart(IDM_PROGRESS);
    if (part >= 0)
    {
        CRect rcPart = GetPartRect(part);
        m_progressBar.SetWindowPos(NULL, rcPart, SWP_SHOWWINDOW);
    }
      // set the hyperlink  into its part
    part = IDtoPart(IDM_STATUS_HLINK);
    if (part >= 0)
    {
        CRect rcPart = GetPartRect(part);
        m_hyperlink.SetWindowPos(NULL, rcPart, SWP_SHOWWINDOW);
    }
}

/*============================================================================*/
    void CustomStatusBar::
PreCreate(CREATESTRUCT& cs)                                                 /*

    Adjust the style parameters of the custom statusbar.
*-----------------------------------------------------------------------------*/
{
    cs.style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | CCS_BOTTOM |
        WS_CLIPCHILDREN;  // to reduce flicker on resizing

      // Suppress the gripper unless XP themes are enabled
    if (IsXPThemed())
        cs.style |= SBARS_SIZEGRIP;
}

/*============================================================================*/
    void    CustomStatusBar::
SetOwnerDrawText(LPCTSTR odmsg)                                             /*

    Enter the odmsg string into the owner-drawn part of the custom status bar.
*-----------------------------------------------------------------------------*/
{
    m_ownerDrawMsg = odmsg;
    SetStatusIndicators();
}

/*============================================================================*/
    void CustomStatusBar::
SetStatusIndicators()                                                       /*

    Place string, icon, and custom resources into their designated status
    bar parts. Designate owner drawn parts to be filled by the main frame
    OnDraw() method. Recompute the sizes of parts, as these may have
    changed due to resizing.
*-----------------------------------------------------------------------------*/
{
    UINT idCount = static_cast<UINT>(m_statusBarParts.size());
    if (!IsWindow() || idCount == 0)
        return;

      // load panes with specified id strings, count the number of
      // SB_AUTOSIZE's, and calculate committed width
    UINT autoWidths = 0;
      // make room for a gripper control if the style says it's there
    UINT gripper = (GetStyle() & SBARS_SIZEGRIP) != 0 ? GRIPPER_SIZE : 0;
    UINT cumulativeWidths = gripper; // space for gripper, if present
    for (UINT i = 0; i < idCount; i++)
    {
        PartInfo part = m_statusBarParts[i];
        UINT id = part.ID;
          // set the pane size for icons and strings
        CString s = LoadString(id);
        HICON h = (HICON)GetApp()->LoadImage(id, IMAGE_ICON,
            16, 16, LR_DEFAULTCOLOR | LR_SHARED);
        if (h != NULL && part.width == SB_AUTOSIZE)
        {     // icon widths are square, so use height
            UINT width = GetPartRect(0).Height();
            m_statusBarParts[i].width = width + BORDER_SIZE;
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
            m_statusBarParts[i].width +=  rc.Width() + BORDER_SIZE;
        }
          // count remaining auto-sized parts and committed width
        part.width = m_statusBarParts[i].width;
        if (part.width == SB_AUTOSIZE)
            autoWidths++;
        else
            cumulativeWidths += part.width;
    }
      // distribute available width among auto_sized panes
    int auto_size = autoWidths > 0 ?
        (GetClientRect().Width() - cumulativeWidths) / autoWidths : 0;
      // set part, icons, and owner drawn parts
    for (UINT j = 0; j < idCount; j++)
    {
        BOOL skip = FALSE; // skip if status is not of use
        PartInfo part = m_statusBarParts[j];
        UINT id = part.ID;
          // if this id has an icon, set in the part, but only if
          // different than the current icon.
        HICON curIcon = GetPartIcon(j),
              idIcon  = (HICON)GetApp()->LoadImage(id, IMAGE_ICON, 16, 16,
                        LR_DEFAULTCOLOR | LR_SHARED);
        if (idIcon != NULL && curIcon != idIcon)
            SetPartIcon(j, idIcon);
          // evaluate strings and owner drawn
        CString status,
            idString = LoadString(id);
        switch (id)
        {
            case IDW_INDICATOR_CAPS:
            status = (::GetKeyState(VK_CAPITAL) & 0x0001) ?
                idString : CString(_T(" "));
            break;

            case IDW_INDICATOR_NUM:
            status = (::GetKeyState(VK_NUMLOCK) & 0x0001)?
                idString : CString(_T(" "));
            break;

            case IDM_INDICATOR_INS:
            status = (::GetKeyState(VK_INSERT) & 0x0001)?
                idString : CString(_T(" "));
            break;

            case IDW_INDICATOR_SCRL:
            status = (::GetKeyState(VK_SCROLL) & 0x0001)?
                idString : CString(_T(" "));
            break;

            default:
            if (part.style & SBT_OWNERDRAW)
            {    // Set this part as owner drawn. The drawing
                 // is performed in OnMessageReflect().
                status = m_ownerDrawMsg;
            }
            else
            {     // if the part id has a resource string,
                   //use it; otherwise, do not alter this entry
                status = idString;
                skip = idString.IsEmpty();
            }
            break;
        }
          // reset the size in case the width changed
        int size = (part.width == SB_AUTOSIZE ?
            auto_size : m_statusBarParts[j].width);
        SetPartWidth(j, size);
          // reset indicators only if they changed
        if (!skip && status != m_statusBarParts[j].old)
        {
            SetPartText(j, status, part.style);
            m_statusBarParts[j].old = status;
        }
    }
      // reset parts requiring relocation
    PositionCustomParts();
}

/*============================================================================*/
    LRESULT CustomStatusBar::
WndProc(UINT msg, WPARAM wparam, LPARAM lparam)                            /*

    Process custom statusbar messages.
*-----------------------------------------------------------------------------*/
{
    try
    {
        switch(msg)
        {
            case WM_TIMER:
            {
                  // Change the Progress Bar indicator
                m_progressBar.OffsetPos(1);
                if (m_progressBar.GetRange(FALSE) == m_progressBar.GetPos())
                    m_progressBar.SetPos(0);
                break;
            }
        }
        return WndProcDefault(msg, wparam, lparam);
    }
      // Catch all CException types.
    catch (const CException& e)
    {
          // Display the exception and continue.
        ::MessageBox(0, e.GetText(), AtoT(e.what()), MB_ICONERROR);
        return 0;
    }
}
/*----------------------------------------------------------------------------*/