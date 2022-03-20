

#include "stdafx.h"
#include "PrintDialog.h"


////////////////////////////////////////
// CMyPrintDialogEx function definitions
//

// Called when the the dialog is initialized.
BOOL CMyPrintDialogEx::OnInitDialog()
{
    // Setup the CPrintParent object.
    m_printParent.Attach(GetParent());
    m_printParent.SetOwner(GetParameters().hwndOwner);

    return TRUE;
}