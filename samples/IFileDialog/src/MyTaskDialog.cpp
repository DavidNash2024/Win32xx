/////////////////////////////
// MyTaskDialog.cpp
//

#include "stdafx.h"
#include "MyTaskDialog.h"
#include "IFileDialog.h"
#include "resource.h"


//////////////////////////////////////
// CMyTaskDialog function definitions.
//

CMyTaskDialog::CMyTaskDialog()
{
    SetOptions(TDF_USE_COMMAND_LINKS | TDF_ALLOW_DIALOG_CANCELLATION);
    SetMainInstruction(L"Pick the file dialog sample you want to try");
    SetWindowTitle(L"Common File Dialog");

    // Add the commands.
    AddCommandControl(IDC_CHOOSEFILE,                    L"Choose File");
    AddCommandControl(IDC_CHOOSEFOLDER,                  L"Choose Folder");
    AddCommandControl(IDC_CHOOSEMULTIPLEFILES,           L"Choose Multiple Files");
    AddCommandControl(IDC_ADDCUSTOMCONTROLS,             L"Add Custom Controls");
    AddCommandControl(IDC_SETDEFAULTVALUESFORPROPERTIES, L"Set Properties for File Type");
    AddCommandControl(IDC_WRITEPROPERTIESWITHHANDLERS,   L"Write Properties Using Handlers");
    AddCommandControl(IDC_WRITEPROPERTIESWITHOUTHANDLERS, L"Write Properties without Using Handlers");

    // Set Icons
    SetMainIcon(GetApp()->LoadIcon(IDW_MAIN));
}

// Called when the user selects a button or command link.
BOOL CMyTaskDialog::OnTDButtonClicked(int buttonID)
{
    switch (buttonID)
    {
    case IDC_CHOOSEFILE:
        ChooseFile();
        return TRUE;
    case IDC_CHOOSEFOLDER:
        ChooseFolder();
        return TRUE;
    case IDC_CHOOSEMULTIPLEFILES:
        ChooseMultipleFiles();
        return TRUE;
    case IDC_ADDCUSTOMCONTROLS:
        AddCustomControls();
        return TRUE;
    case IDC_SETDEFAULTVALUESFORPROPERTIES:
        SetDefaultValuesForProperties();
        return TRUE;
    case IDC_WRITEPROPERTIESWITHHANDLERS:
        WritePropertiesUsingHandlers();
        return TRUE;
    case IDC_WRITEPROPERTIESWITHOUTHANDLERS:
        WritePropertiesWithoutUsingHandlers();
        return TRUE;
    case IDCANCEL:
        return FALSE;
    }

    // Return FALSE to close the dialog.
    // Return TRUE to prevent the task dialog from closing.
    return FALSE;
}

// Processes the task dialog's messages.
LRESULT CMyTaskDialog::TaskDialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        return TaskDialogProcDefault(msg, wparam, lparam);
    }

    // Catch all unhandled CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << L'\n' << e.GetErrorString();

        CString str2;
        str2 << "Error: " << e.what();
        TaskDialogBox(nullptr, str1, str2, TD_ERROR_ICON);
    }

    // Catch all unhandled std::exception types.
    catch (const std::exception& e)
    {
        // Display the exception and continue.
        CString str1 = e.what();
        TaskDialogBox(nullptr, str1, L"Error: std::exception", TD_ERROR_ICON);
    }

    return 0;
}