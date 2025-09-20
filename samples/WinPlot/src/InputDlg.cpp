/////////////////////////////
// InputDlg.cpp
//

#include "stdafx.h"
#include "InputDlg.h"

//////////////////////////////////
// CInputDlg function definitions.
//
void  CInputDlg::DoDataExchange(CDataExchange& DX)
{
    // connect xMin edit box.
    DX.DDX_Text(IDC_XMIN, m_xMin);
    DX.DDV_MinMaxDouble(m_xMin, -100000.0, 100000.0);

    // connect xMax edit box.
    DX.DDX_Text(IDC_XMAX, m_xMax);
    DX.DDV_MinMaxDouble(m_xMax, -100000.0, 100000.0);

    // connect function input box.
    DX.DDX_Text(IDC_FUNCTION_INPUT, m_function);
    DX.DDV_MaxChars(m_function, 255);
}

BOOL CInputDlg::OnInitDialog()
{
    // Connect controls to IDs and read default data values into them.
    CDataExchange DX;
    UpdateData(DX, SENDTOCONTROL);
    return TRUE;
}

void CInputDlg::OnOK()
{
    CDataExchange DX;
    UpdateData(DX, READFROMCONTROL);
    CDialog::OnOK();
}

// Process the dialog's window messages.
INT_PTR CInputDlg::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
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
