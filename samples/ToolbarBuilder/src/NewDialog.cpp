/////////////////////////////
// NewDialog.cpp
//

#include "stdafx.h"
#include "NewDialog.h"
#include "resource.h"

/////////////////////////////////
// CNewDialog function definitions
//

// Constructor.
CNewDialog::CNewDialog(UINT resID) : CDialog(resID),
    m_imageSizeRadio(IDC_RADIO2), m_colorBitsRadio(IDC_RADIO10)
{
}

// Process the dialog's window messages.
INT_PTR CNewDialog::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
    //  switch (msg)
    //  {
    //  Additional messages to be handled go here
    //  }

        // Pass unhandled messages on to parent DialogProc.
        return DialogProcDefault(msg, wparam, lparam);
    }

    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << L'\n' << e.GetErrorString();

        CString str2;
        str2 << "Error: " << e.what();
        ::MessageBox(nullptr, str1, str2, MB_ICONERROR);
    }

    // Catch all unhandled std::exception types.
    catch (const std::exception& e)
    {
        // Display the exception and continue.
        CString str1 = e.what();
        ::MessageBox(nullptr, str1, L"Error: std::exception", MB_ICONERROR);
    }

    return 0;
}

int CNewDialog::GetColorBits() const
{
    int colorChoice = m_colorBitsRadio - IDC_RADIO10 + 1;
    switch (colorChoice)
    {
    case 1: return 32;
    case 2: return 24;
    case 3: return 8;
    }

    return 32;
}

int CNewDialog::GetImageSize() const
{
    int sizeChoice = m_imageSizeRadio - IDC_RADIO1 + 1;
    switch (sizeChoice)
    {
    case 1: return 16;
    case 2: return 24;
    case 3: return 32;
    case 4: return 48;
    case 5: return 64;
    case 6: return 96;
    case 7: return 128;
    case 8: return 192;
    case 9: return 256;
    }

    return 24;
}

// Handle the command notifications from controls.
BOOL CNewDialog::OnCommand(WPARAM wparam, LPARAM)
{
    UINT id = LOWORD(wparam);
    switch (id)
    {
    case IDC_RADIO1:    // intentionally blank
    case IDC_RADIO2:    // intentionally blank
    case IDC_RADIO3:    // intentionally blank
    case IDC_RADIO4:    // intentionally blank
    case IDC_RADIO5:    // intentionally blank
    case IDC_RADIO6:    // intentionally blank
    case IDC_RADIO7:    // intentionally blank
    case IDC_RADIO8:    // intentionally blank
    case IDC_RADIO9:    return OnRangeOfImageSizes(IDC_RADIO1, IDC_RADIO9, id);

    case IDC_RADIO10:   // intentionally blank
    case IDC_RADIO11:   // intentionally blank
    case IDC_RADIO12:   return OnRangeOfColorBits(IDC_RADIO10, IDC_RADIO12, id);
    }

    return FALSE;
}

// Called before the dialog is displayed.
BOOL CNewDialog::OnInitDialog()
{
    // Select the first image size radio button.
    CheckRadioButton(IDC_RADIO1, IDC_RADIO9, m_imageSizeRadio);

    // Select the first color format radio button.
    CheckRadioButton(IDC_RADIO10, IDC_RADIO12, m_colorBitsRadio);

    return TRUE;
}

// Called when an image size radio button is selected.
bool CNewDialog::OnRangeOfImageSizes(UINT idFirst, UINT idLast, UINT idClicked)
{
    CheckRadioButton(idFirst, idLast, idClicked);
    CheckRadioButton(IDC_RADIO10, IDC_RADIO12, m_colorBitsRadio);
    m_imageSizeRadio = idClicked;

    return  true;
}

// Called when a color format radio button is selected.
bool CNewDialog::OnRangeOfColorBits(UINT idFirst, UINT idLast, UINT idClicked)
{
    CheckRadioButton(IDC_RADIO1, IDC_RADIO9, m_imageSizeRadio);
    CheckRadioButton(idFirst, idLast, idClicked);
    m_colorBitsRadio = idClicked;

    return true;
}
