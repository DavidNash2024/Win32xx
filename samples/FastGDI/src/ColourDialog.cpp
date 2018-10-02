//////////////////////////////////////////////
// ColourDialog.cpp

#include "stdafx.h"
#include "ColourDialog.h"
#include "FastGDIApp.h"
#include "resource.h"


CColourDialog::CColourDialog(UINT resID, CBitmap& image) : CDialog(resID), m_image(image)
{
    m_cRed = 0;
    m_cGreen = 0;
    m_cBlue = 0;
}

CColourDialog::~CColourDialog()
{
}

BOOL CColourDialog::OnCommand(WPARAM wparam, LPARAM /*lparam*/)
{

    UINT id = LOWORD(wparam);
    switch (id)
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
        m_previewImage.GrayScaleBitmap();
    else
    {
        // Copy m_hbmPreviewOrig to m_hbmPreview
        CMemDC Mem1DC(NULL);
        Mem1DC.SelectObject(m_previewOrigImage);
        CMemDC Mem2DC(NULL);
        Mem2DC.SelectObject(m_previewImage);
        int cx = m_preview.GetWindowRect().Width();
        int cy = m_preview.GetWindowRect().Height();
        Mem2DC.BitBlt(0, 0, cx, cy, Mem1DC, 0, 0, SRCCOPY);

        Mem2DC.DetachBitmap();  // Detach bitmap before modifying it
        m_previewImage.TintBitmap(m_cRed, m_cGreen, m_cBlue);
    }

    PaintPreview();
}

BOOL CColourDialog::OnInitDialog()
{
    // Attach the Trackbar controls to CWnd objects
    m_redSlider.AttachDlgItem(IDC_SLIDER_RED, *this);
    m_greenSlider.AttachDlgItem(IDC_SLIDER_GREEN, *this);
    m_blueSlider.AttachDlgItem(IDC_SLIDER_BLUE, *this);

    // Set Trackbar ranges
    m_redSlider.SendMessage(TBM_SETRANGE, TRUE, MAKELONG(-255, 255));
    m_greenSlider.SendMessage(TBM_SETRANGE, TRUE, MAKELONG(-255, 255));
    m_blueSlider.SendMessage(TBM_SETRANGE, TRUE, MAKELONG(-255, 255));

    // Attach the Edit controls to CWnd objects
    m_redEdit.AttachDlgItem(IDC_EDIT_RED, *this);
    m_greenEdit.AttachDlgItem(IDC_EDIT_GREEN, *this);
    m_blueEdit.AttachDlgItem(IDC_EDIT_BLUE, *this);

    m_redEdit.SendMessage(WM_SETTEXT, 0, reinterpret_cast<LPARAM>(_T("0")));
    m_greenEdit.SendMessage(WM_SETTEXT, 0, reinterpret_cast<LPARAM>(_T("0")));
    m_blueEdit.SendMessage(WM_SETTEXT, 0, reinterpret_cast<LPARAM>(_T("0")));

    // Create the two image previews
    m_preview.AttachDlgItem(IDC_PREVIEW, *this);
    CreateImagePreviews();

    return TRUE;
}

INT_PTR CColourDialog::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch(msg)
    {
    case WM_HSCROLL:    return OnHScroll(msg, wparam, lparam);
    case WM_PAINT:      return OnPaint(msg, wparam, lparam);
    }

    return DialogProcDefault(msg, wparam, lparam);
}

LRESULT CColourDialog::OnHScroll(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Processes messages from the slider controls

    UNREFERENCED_PARAMETER(msg);
    UNREFERENCED_PARAMETER(wparam);

    HWND hWnd = reinterpret_cast<HWND>(lparam);

    // Update the text for the colour's edit control
    int nPos = static_cast<int>(SendMessage(hWnd, TBM_GETPOS, 0, 0));
    TCHAR Text[5];
    wsprintf(Text, _T("%d\0"), nPos);

    if (hWnd == m_redSlider)
        m_redEdit.SendMessage(WM_SETTEXT, 0, reinterpret_cast<LPARAM>(&Text));
    else if (hWnd == m_greenSlider)
        m_greenEdit.SendMessage(WM_SETTEXT, 0, reinterpret_cast<LPARAM>(&Text));
    else if (hWnd == m_blueSlider)
        m_blueEdit.SendMessage(WM_SETTEXT, 0, reinterpret_cast<LPARAM>(&Text));

    // Store the colour values
    m_cRed   = static_cast<int>(m_redSlider.SendMessage(TBM_GETPOS));
    m_cGreen = static_cast<int>(m_greenSlider.SendMessage(TBM_GETPOS));
    m_cBlue  = static_cast<int>(m_blueSlider.SendMessage(TBM_GETPOS));

    // Copy m_hbmPreviewOrig to m_hbmPreview
    CMemDC Mem1DC(NULL);
    Mem1DC.SelectObject(m_previewOrigImage);
    CMemDC Mem2DC(NULL);
    Mem2DC.SelectObject(m_previewImage);
    int cx = m_preview.GetWindowRect().Width();
    int cy = m_preview.GetWindowRect().Height();
    Mem2DC.BitBlt(0, 0, cx, cy, Mem1DC, 0, 0, SRCCOPY);

    // Update the colour of the preview image
    Mem2DC.DetachBitmap();  // Detach bitmap before modifying it
    m_previewImage.TintBitmap(m_cRed, m_cGreen, m_cBlue);

    if (SendDlgItemMessage(IDC_CHECK1, BM_GETCHECK, 0, 0))
        m_previewImage.GrayScaleBitmap();

    PaintPreview();

    return 0;
}

void CColourDialog::OnOK()
{
    // Get a pointer to our CMainFrame object
    CMainFrame& mainFrame = GetFrameApp().GetMainFrame();

    BOOL isGray = (SendDlgItemMessage(IDC_CHECK1, BM_GETCHECK, 0, 0) != 0);
    mainFrame.ModifyBitmap(m_cRed, m_cGreen, m_cBlue, isGray);

    CDialog::OnOK();
}

LRESULT CColourDialog::OnPaint(UINT msg, WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(msg);
    UNREFERENCED_PARAMETER(wparam);
    UNREFERENCED_PARAMETER(lparam);

    PaintPreview();
    return 0;
}

void CColourDialog::PaintPreview()
// Displays the bitmap in the display area of our dialog
{
    BITMAP bm;
    m_previewImage.GetObject( sizeof(BITMAP), &bm );

    // Get the size of the destination display area
    CRect rcView = m_preview.GetClientRect();
    m_preview.MapWindowPoints(*this, rcView);

    int leftDest;
    int topDest;
    if (bm.bmWidth < bm.bmHeight*rcView.Width()/rcView.Height())
    {
        leftDest = rcView.left + (rcView.Width() - bm.bmWidth)/2;
        topDest = rcView.top;
    }
    else
    {
        leftDest = rcView.left;
        topDest = rcView.top + (rcView.Height() - bm.bmHeight)/2;
    }

    CClientDC previewDC(*this);
    CMemDC memDC(previewDC);
    memDC.SelectObject(m_previewImage);
    previewDC.BitBlt(leftDest, topDest, bm.bmWidth, bm.bmHeight, memDC, 0, 0, SRCCOPY);
}

void CColourDialog::CreateImagePreviews()
// Creates the two Preview bitmaps: m_Preview and m_PreviewOrig
{
    // Get the size of the bitmap
    BITMAP bm;
    m_image.GetObject(sizeof(BITMAP), &bm);

    // Get the size of the destination display area
    CRect rcView = m_preview.GetClientRect();
    m_preview.MapWindowPoints(*this, (LPPOINT)&rcView, 2);

    // Calculate the stretch values, preserving the aspect ratio
    int widthDest;
    int heightDest;
    double aspectRatio;

    if (bm.bmWidth < bm.bmHeight*rcView.Width()/rcView.Height())
    {
        aspectRatio = static_cast<double>(bm.bmWidth) / static_cast<double>(bm.bmHeight);
        widthDest = static_cast<int>(rcView.Height()*aspectRatio);
        heightDest = rcView.Height();
    }
    else
    {
        aspectRatio = static_cast<double>(bm.bmHeight) / static_cast<double>(bm.bmWidth);
        widthDest = rcView.Width();
        heightDest = static_cast<int>(rcView.Width()*aspectRatio);
    }

    // Create the Device Contexts and compatible bitmaps
    CMemDC dest1DC(NULL);
    CMemDC dest2DC(NULL);
    CMemDC memDC(NULL);
    CClientDC desktopDC(NULL);
    m_previewImage.CreateCompatibleBitmap(desktopDC, widthDest, heightDest);
    m_previewOrigImage.CreateCompatibleBitmap(desktopDC, widthDest, heightDest);
    memDC.SelectObject(m_image);
    dest1DC.SelectObject(m_previewImage);
    dest2DC.SelectObject(m_previewOrigImage);

    // Stretch the bitmap to fit in the destination display area
    dest1DC.SetStretchBltMode(COLORONCOLOR);
    dest1DC.StretchBlt(0, 0, widthDest, heightDest, memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

    // Make a second copy of the bitmap
    dest2DC.BitBlt(0, 0, widthDest, heightDest, dest1DC, 0, 0, SRCCOPY);
}

