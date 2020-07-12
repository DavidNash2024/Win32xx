
#include "stdafx.h"
#include "InputDlg.h"


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

