//////////////////////////////////////////////
// ColourDialog.cpp

#include "stdafx.h"
#include "ColourDialog.h"
#include "FastGDIApp.h"
#include "resource.h"


CColourDialog::CColourDialog(UINT nResID, CBitmap& bmImage) : CDialog(nResID), m_bmImage(bmImage)
{
    m_cRed = 0;
    m_cGreen = 0;
    m_cBlue = 0;
}

CColourDialog::~CColourDialog()
{
}

BOOL CColourDialog::OnCommand(WPARAM wParam, LPARAM /*lParam*/)
{

    UINT nID = LOWORD(wParam);
    switch (nID)
    {
    case IDC_CHECK1:
        OnGrayScale();
        return TRUE;    // return TRUE for handled commands
    }

    // return FALSE for unhandled commands
    return FALSE;
}

void CColourDialog::OnGrayScale()
{
    // Update the colour of the preview image
    if (SendDlgItemMessage(IDC_CHECK1, BM_GETCHECK, 0, 0))
        m_bmPreview.GrayScaleBitmap();
    else
    {
        // Copy m_hbmPreviewOrig to m_hbmPreview
        CMemDC Mem1DC(NULL);
        Mem1DC.SelectObject(m_bmPreviewOrig);
        CMemDC Mem2DC(NULL);
        Mem2DC.SelectObject(m_bmPreview);
        int cx = m_Preview.GetWindowRect().Width();
        int cy = m_Preview.GetWindowRect().Height();
        Mem2DC.BitBlt(0, 0, cx, cy, Mem1DC, 0, 0, SRCCOPY);

        Mem2DC.DetachBitmap();  // Detach bitmap before modifying it
        m_bmPreview.TintBitmap(m_cRed, m_cGreen, m_cBlue);
    }

    PaintPreview();
}

BOOL CColourDialog::OnInitDialog()
{
    // Attach the Trackbar controls to CWnd objects
    m_RedSlider.AttachDlgItem(IDC_SLIDER_RED, *this);
    m_GreenSlider.AttachDlgItem(IDC_SLIDER_GREEN, *this);
    m_BlueSlider.AttachDlgItem(IDC_SLIDER_BLUE, *this);

    // Set Trackbar ranges
    m_RedSlider.SendMessage(TBM_SETRANGE, TRUE, MAKELONG(-255, 255));
    m_GreenSlider.SendMessage(TBM_SETRANGE, TRUE, MAKELONG(-255, 255));
    m_BlueSlider.SendMessage(TBM_SETRANGE, TRUE, MAKELONG(-255, 255));

    // Attach the Edit controls to CWnd objects
    m_RedEdit.AttachDlgItem(IDC_EDIT_RED, *this);
    m_GreenEdit.AttachDlgItem(IDC_EDIT_GREEN, *this);
    m_BlueEdit.AttachDlgItem(IDC_EDIT_BLUE, *this);

    m_RedEdit.SendMessage(WM_SETTEXT, 0, reinterpret_cast<LPARAM>(_T("0")));
    m_GreenEdit.SendMessage(WM_SETTEXT, 0, reinterpret_cast<LPARAM>(_T("0")));
    m_BlueEdit.SendMessage(WM_SETTEXT, 0, reinterpret_cast<LPARAM>(_T("0")));

    // Create the two image previews
    m_Preview.AttachDlgItem(IDC_PREVIEW, *this);
    CreateImagePreviews();

    return TRUE;
}

INT_PTR CColourDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_HSCROLL:    return OnHScroll(uMsg, wParam, lParam);
    case WM_PAINT:      return OnPaint(uMsg, wParam, lParam);
    }

    return DialogProcDefault(uMsg, wParam, lParam);
}

LRESULT CColourDialog::OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // Processes messages from the slider controls

    UNREFERENCED_PARAMETER(uMsg);
    UNREFERENCED_PARAMETER(wParam);

    HWND hWnd = reinterpret_cast<HWND>(lParam);

    // Update the text for the colour's edit control
    int nPos = SendMessage(hWnd, TBM_GETPOS, 0, 0);
    TCHAR Text[5];
    wsprintf(Text, _T("%d\0"), nPos);

    if (hWnd == m_RedSlider)
        m_RedEdit.SendMessage(WM_SETTEXT, 0, reinterpret_cast<LPARAM>(&Text));
    else if (hWnd == m_GreenSlider)
        m_GreenEdit.SendMessage(WM_SETTEXT, 0, reinterpret_cast<LPARAM>(&Text));
    else if (hWnd == m_BlueSlider)
        m_BlueEdit.SendMessage(WM_SETTEXT, 0, reinterpret_cast<LPARAM>(&Text));

    // Store the colour values
    m_cRed   = m_RedSlider.SendMessage(TBM_GETPOS);
    m_cGreen = m_GreenSlider.SendMessage(TBM_GETPOS);
    m_cBlue  = m_BlueSlider.SendMessage(TBM_GETPOS);

    // Copy m_hbmPreviewOrig to m_hbmPreview
    CMemDC Mem1DC(NULL);
    Mem1DC.SelectObject(m_bmPreviewOrig);
    CMemDC Mem2DC(NULL);
    Mem2DC.SelectObject(m_bmPreview);
    int cx = m_Preview.GetWindowRect().Width();
    int cy = m_Preview.GetWindowRect().Height();
    Mem2DC.BitBlt(0, 0, cx, cy, Mem1DC, 0, 0, SRCCOPY);

    // Update the colour of the preview image
    Mem2DC.DetachBitmap();  // Detach bitmap before modifying it
    m_bmPreview.TintBitmap(m_cRed, m_cGreen, m_cBlue);

    if (SendDlgItemMessage(IDC_CHECK1, BM_GETCHECK, 0, 0))
        m_bmPreview.GrayScaleBitmap();

    PaintPreview();

    return 0L;
}

void CColourDialog::OnOK()
{
    // Get a pointer to our CMainFrame object
    CMainFrame& MainFrame = GetFrameApp().GetMainFrame();

    BOOL bGray = SendDlgItemMessage(IDC_CHECK1, BM_GETCHECK, 0, 0);
    MainFrame.ModifyBitmap(m_cRed, m_cGreen, m_cBlue, bGray);

    CDialog::OnOK();
}

LRESULT CColourDialog::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(uMsg);
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);

    PaintPreview();
    return 0L;
}

void CColourDialog::PaintPreview()
// Displays the bitmap in the display area of our dialog
{
    BITMAP bm;
    m_bmPreview.GetObject( sizeof(BITMAP), &bm );

    // Get the size of the destination display area
    CRect rcView = m_Preview.GetClientRect();
    m_Preview.MapWindowPoints(*this, rcView);

    int nLeftDest;
    int nTopDest;
    if (bm.bmWidth < bm.bmHeight*rcView.Width()/rcView.Height())
    {
        nLeftDest = rcView.left + (rcView.Width() - bm.bmWidth)/2;
        nTopDest = rcView.top;
    }
    else
    {
        nLeftDest = rcView.left;
        nTopDest = rcView.top + (rcView.Height() - bm.bmHeight)/2;
    }

    CClientDC dcPreview(*this);
    CMemDC MemDC(dcPreview);
    MemDC.SelectObject(m_bmPreview);
    dcPreview.BitBlt(nLeftDest, nTopDest, bm.bmWidth, bm.bmHeight, MemDC, 0, 0, SRCCOPY);
}

void CColourDialog::CreateImagePreviews()
// Creates the two Preview bitmaps: m_Preview and m_PreviewOrig
{
    // Get the size of the bitmap
    BITMAP bm;
    m_bmImage.GetObject(sizeof(BITMAP), &bm);

    // Get the size of the destination display area
    CRect rcView = m_Preview.GetClientRect();
    m_Preview.MapWindowPoints(*this, (LPPOINT)&rcView, 2);

    // Calculate the stretch values, preserving the aspect ratio
    int nWidthDest;
    int nHeightDest;
    double AspectRatio;

    if (bm.bmWidth < bm.bmHeight*rcView.Width()/rcView.Height())
    {
        AspectRatio = static_cast<double>(bm.bmWidth) / static_cast<double>(bm.bmHeight);
        nWidthDest = static_cast<int>(rcView.Height()*AspectRatio);
        nHeightDest = rcView.Height();
    }
    else
    {
        AspectRatio = static_cast<double>(bm.bmHeight) / static_cast<double>(bm.bmWidth);
        nWidthDest = rcView.Width();
        nHeightDest = static_cast<int>(rcView.Width()*AspectRatio);
    }

    // Create the Device Contexts and compatible bitmaps
    CMemDC Dest1DC(NULL);
    CMemDC Dest2DC(NULL);
    CMemDC MemDC(NULL);
    CClientDC dcDesktop(NULL);
    m_bmPreview.CreateCompatibleBitmap(dcDesktop, nWidthDest, nHeightDest);
    m_bmPreviewOrig.CreateCompatibleBitmap(dcDesktop, nWidthDest, nHeightDest);
    MemDC.SelectObject(m_bmImage);
    Dest1DC.SelectObject(m_bmPreview);
    Dest2DC.SelectObject(m_bmPreviewOrig);

    // Stretch the bitmap to fit in the destination display area
    Dest1DC.SetStretchBltMode(COLORONCOLOR);
    Dest1DC.StretchBlt(0, 0, nWidthDest, nHeightDest, MemDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

    // Make a second copy of the bitmap
    Dest2DC.BitBlt(0, 0, nWidthDest, nHeightDest, Dest1DC, 0, 0, SRCCOPY);
}

