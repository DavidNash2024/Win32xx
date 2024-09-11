/////////////////////////////
// AppHelp.cpp
//

#include "stdafx.h"
#include "AppHelp.h"
#include "resource.h"

/////////////////////////////
// CHelp function definitions
//

// Constructor.
CHelp::CHelp()
{
    SetCommandHelpTopics();
    SetClientHelpTopics();
    SetFrameHelpTopics();
}

// Opens the help about dialog.
BOOL CHelp::About(HWND hParent)
{
    m_helpAbout.DoModal(hParent);
    return TRUE;
}

// Adds a topic to the context help map.
void CHelp::AddHelpTopic(UINT id, LPCWSTR topic)
{
    m_contextHelp.AddHelpTopic(id, topic);
}

// Define the context help topics to be displayed for the menu items and the toolbar.
void CHelp::SetCommandHelpTopics()
{
    // define the help topics for the menu and status bar
    AddHelpTopic(IDW_MENUBAR,            L"ToolbarandMenuItems");
    AddHelpTopic(IDW_STATUSBAR,          L"StatusbarTopics");

    // define the help topics for the tool bar buttons
    AddHelpTopic(IDM_FILE_NEW,           L"NewDocument");
    AddHelpTopic(IDM_FILE_OPEN,          L"OpenExistingDocument");
    AddHelpTopic(IDM_FILE_SAVE,          L"SaveCurrentDocument");
    AddHelpTopic(IDM_FILE_SAVEAS,        L"SaveAsAnotherDocument");
    AddHelpTopic(IDM_FILE_CLOSE,         L"CloseCurrentDocument");
    AddHelpTopic(IDM_FILE_PRINT,         L"PrintDocument");
    AddHelpTopic(IDM_FILE_PRINT_PREVIEW, L"PreviewPrintout");
    AddHelpTopic(IDM_FILE_EXIT,          L"ExitTerminateProgram");
    AddHelpTopic(IDM_EDIT_UNDO,          L"UndoFunction");
    AddHelpTopic(IDM_EDIT_REDO,          L"RedoFunction");
    AddHelpTopic(IDM_EDIT_CUT,           L"CutFunction");
    AddHelpTopic(IDM_EDIT_COPY,          L"CopyFunction");
    AddHelpTopic(IDM_EDIT_PASTE,         L"PasteFunction");
    AddHelpTopic(IDM_EDIT_DELETE,        L"DeleteFunction");
    AddHelpTopic(IDM_EDIT_FIND,          L"FindInDocument");
    AddHelpTopic(IDM_EDIT_REPLACE,       L"ReplaceInDocument");
    AddHelpTopic(IDM_HELP_CONTENT,       L"Introduction");
    AddHelpTopic(IDM_HELP_CONTEXT,       L"Welcome");
    AddHelpTopic(IDM_HELP_ABOUT,         L"AboutThisProgram");
    AddHelpTopic(IDM_COLOR_CHOICE,       L"ColorChoiceFunction");
    AddHelpTopic(IDM_FONT_CHOICE,        L"FontChoiceFunction");
    AddHelpTopic(IDW_ABOUT,              L"AboutThisProgram");
}

// Define the help topic for the dialog.
void CHelp::SetClientHelpTopics()
{
    // Define the help topic for the dialog's client area.
    AddHelpTopic(IDD_DIALOG1,       L"clientwindowcontrols");

    // Define the help topics for the client area controls.
    AddHelpTopic(IDC_LIST1,         L"ListBoxDialogHelp");
    AddHelpTopic(IDC_RICHEDIT1,     L"RichEditBoxUsage");
    AddHelpTopic(IDC_EDIT1,         L"EditBoxUsage");
    AddHelpTopic(ID_RADIO_A,        L"RadioButtonUsage");
    AddHelpTopic(ID_RADIO_B,        L"RadioButtonUsage");
    AddHelpTopic(ID_RADIO_C,        L"RadioButtonUsage");
    AddHelpTopic(ID_CHECK_A,        L"CheckBoxUsage");
    AddHelpTopic(ID_CHECK_B,        L"CheckBoxUsage");
    AddHelpTopic(ID_CHECK_C,        L"CheckBoxUsage");
    AddHelpTopic(IDC_BUTTON1,       L"Button1Usage");
    AddHelpTopic(IDC_STATUS,        L"StatusBoxContent");
    AddHelpTopic(IDC_BITMAP1,       L"BitmapContent");
    AddHelpTopic(IDOK,              L"OKButtonUsage");
}

// Define the context help topics for the frame.
void CHelp::SetFrameHelpTopics()
{
    AddHelpTopic(IDFR_CLOSE,        L"ExitTerminateProgram");
    AddHelpTopic(IDFR_SYSTEMMENU,   L"SystemMenu");
    AddHelpTopic(IDFR_CAPTION,      L"AppTitleBar");
    AddHelpTopic(IDFR_GRIPPER,      L"ResizeGripper");
    AddHelpTopic(IDFR_HELP,         L"TitleBarQuestionBox");
}

// Assigns the context help file's path name.
void CHelp::SetHelpFilePath(LPCWSTR chmName)
{
    m_contextHelp.SetHelpFilePath(chmName);
}

// Display the context help for the specified topic.
void CHelp::ShowHelpTopic(UINT id)
{
    m_contextHelp.ShowHelpTopic(id);
}

// Display the context help for the specified topic.
void CHelp::ShowHelpTopic(LPCWSTR topic)
{
    m_contextHelp.ShowHelpTopic(topic);
}

// Assigns the credits text for the help about dialog.
void CHelp::SetCredits(LPCWSTR credits)
{
    m_helpAbout.SetCredits(credits);
}
