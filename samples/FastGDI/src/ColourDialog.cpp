//////////////////////////////////////////////
// ColourDialog.cpp

#include "stdafx.h"
#include "ColourDialog.h"
#include "resource.h"

/////////////////////////////////////
// CColourDialog function definitions
//

// Constructor.
CColourDialog::CColourDialog(UINT resID, CBitmap& image) : CDialog(resID), m_image(image),
                              m_cBlue(0), m_cGreen(0), m_cRed(0), m_isGray(FALSE)
{
}

// Destructor.
CColourDialog::~CColourDialog()
{
}

// Creates the two Preview bitmaps: m_Preview and m_PreviewOrig
void CColourDialog::CreateImagePreviews()
{
    // Get the size of the bitmap
    BITMAP bm = m_image.GetBitmapData();

    // Get the size of the destination display area
    CRect rcView = m_preview.GetClientRect();
    m_preview.MapWindowPoints(*this, (LPPOINT)&rcView, 2);

    // Calculate the stretch values, preserving the aspect ratio
    int widthDest;
    int heightDest;
    double aspectRatio;

    if (bm.bmWidth < bm.bmHeight*rcView.Width() / rcView.Height())
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
    CMemDC dest1DC(0);
    CMemDC dest2DC(0);
    CMemDC memDC(0);
    CClientDC desktopDC(0);
    m_previewImage.CreateCompatibleBitmap(desktopDC, widthDest, heightDest);
    m_previewOrigImage.CreateCompatibleBitmap(desktopDC, widthDest, heightDest);
    memDC.SelectObject(m_image);
    dest1DC.SelectObject(m_previewImage);
    dest2DC.SelectObject(m_previewOrigImage);

    // Stretch the bitmap to fit in the destination display area
    dest1DC.SetStretchBltMode(COLORONCOLOR);
    VERIFY(dest1DC.StretchBlt(0, 0, widthDest, heightDest, memDC, 0, 0,
           bm.bmWidth, bm.bmHeight, SRCCOPY));

    // Make a second copy of the bitmap
    VERIFY(dest2DC.BitBlt(0, 0, widthDest, heightDest, dest1DC, 0, 0, SRCCOPY));
}

// Process the dialog's window messages.
INT_PTR CColourDialog::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_HSCROLL:    return OnHScroll(msg, wparam, lparam);
        case WM_PAINT:      return OnPaint(msg, wparam, lparam);
        }

        return DialogProcDefault(msg, wparam, lparam);
    }

    // Catch all CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        ::MessageBox(0, e.GetText(), AtoT(e.what()), MB_ICONERROR);

        return 0;
    }
}

// Process the command messages (WM_COMMAND) from the dialog's controls.
BOOL CColourDialog::OnCommand(WPARAM wparam, LPARAM lparam)
{
    // Notification from the GrayScale check box.
    switch (LOWORD(wparam))
    {
    case IDC_CHECK1:   return OnGrayScale();
    }

    // Notifications of text change for the edit controls.
    switch (HIWORD(wparam))
    {
    case EN_CHANGE:    return OnTextChange(HWND(lparam));
    }

    // return FALSE for unhandled commands
    return FALSE;
}

// Called when the GrayScale check button is clicked
BOOL CColourDialog::OnGrayScale()
{
    // Update the colour of the preview image
    UpdatePreview();
    return TRUE;
}

// Processes messages from the slider controls.
LRESULT CColourDialog::OnHScroll(UINT, WPARAM, LPARAM lparam)
{
    HWND hWnd = reinterpret_cast<HWND>(lparam);

    // Update the text for the colour's edit control
    int nPos = static_cast<int>(SendMessage(hWnd, TBM_GETPOS, 0, 0));
    TCHAR Text[5];
    wsprintf(Text, _T("%d\0"), nPos);

    if (hWnd == m_redSlider)
        m_redEdit.SetWindowText(Text);
    else if (hWnd == m_greenSlider)
        m_greenEdit.SetWindowText(Text);
    else if (hWnd == m_blueSlider)
        m_blueEdit.SetWindowText(Text);

    UpdatePreview();

    return 0;
}

// Called before the dialog is displayed.
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

    m_redEdit.SetWindowText(_T("0"));
    m_greenEdit.SetWindowText(_T("0"));
    m_blueEdit.SetWindowText(_T("0"));

    // Create the two image previews
    m_preview.AttachDlgItem(IDC_PREVIEW, *this);
    CreateImagePreviews();

    return TRUE;
}

LRESULT CColourDialog::OnPaint(UINT, WPARAM, LPARAM)
{
    Paint();
    return 0;
}

// A value in an edit control has been changed.
BOOL CColourDialog::OnTextChange(HWND editCtrl)
{
    if (IsLeftButtonDown())
        return FALSE;

    CString str;;
    str.GetWindowText(editCtrl);
    int value = _ttoi(str);

    // Update the slider to the value entered in its edit control.
    if (editCtrl == m_redEdit)
        m_redSlider.SetPos(value, TRUE);

    if (editCtrl == m_greenEdit)
        m_greenSlider.SetPos(value, TRUE);

    if (editCtrl == m_blueEdit)
        m_blueSlider.SetPos(value, TRUE);

    if (m_previewImage.GetHandle() != 0)
        UpdatePreview();

    return TRUE;
}

// Displays the bitmap in the preview area of our dialog.
void CColourDialog::Paint()
{
    BITMAP bm = m_previewImage.GetBitmapData();

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

// Updates the preview image according to the dialog input.
void CColourDialog::UpdatePreview()
{
    // Copy m_hbmPreviewOrig to m_hbmPreview
    CMemDC Mem1DC(0);    // Compatible with the desktop
    Mem1DC.SelectObject(m_previewOrigImage);
    CMemDC Mem2DC(0);    // Compatible with the desktop
    Mem2DC.SelectObject(m_previewImage);
    int cx = m_preview.GetWindowRect().Width();
    int cy = m_preview.GetWindowRect().Height();
    Mem2DC.BitBlt(0, 0, cx, cy, Mem1DC, 0, 0, SRCCOPY);
    m_previewImage = Mem2DC.DetachBitmap();  // Detach bitmap before modifying it

    m_cBlue  = m_blueSlider.GetPos();
    m_cGreen = m_greenSlider.GetPos();
    m_cRed   = m_redSlider.GetPos();
    m_isGray = (SendDlgItemMessage(IDC_CHECK1, BM_GETCHECK, 0, 0) != 0);

    if (IsGray())
        m_previewImage.GrayScaleBitmap();
    else
        m_previewImage.TintBitmap(GetRed(), GetGreen(), GetBlue());

    Paint();
}

