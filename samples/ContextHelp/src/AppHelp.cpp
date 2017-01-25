
#include "stdafx.h"
#include "AppHelp.h"
#include "resource.h"

CAppHelp::CAppHelp()
{
	SetCommandHelpTopics();
	SetClientHelpTopics();
	SetFrameHelpTopics();
}

void CAppHelp::SetCommandHelpTopics()
// Define the context help topics to be displayed for the menu items and the toolbar.
{
	// define the tool bar button and menu item topics
	m_ContextHelp.AddHelpTopic(IDW_MENUBAR, _T("ToolbarandMenuItems"));
	m_ContextHelp.AddHelpTopic(IDW_STATUSBAR, _T("StatusbarTopics"));
	m_ContextHelp.AddHelpTopic(IDM_FILE_NEW, _T("NewDocument"));
	m_ContextHelp.AddHelpTopic(IDM_FILE_OPEN, _T("OpenExistingDocument"));
	m_ContextHelp.AddHelpTopic(IDM_FILE_SAVE, _T("SaveCurrentDocument"));
	m_ContextHelp.AddHelpTopic(IDM_FILE_SAVEAS, _T("SaveAsAnotherDocument"));
	m_ContextHelp.AddHelpTopic(IDM_FILE_CLOSE, _T("CloseCurrentDocument"));
	m_ContextHelp.AddHelpTopic(IDM_FILE_PRINT, _T("PrintDocument"));
	m_ContextHelp.AddHelpTopic(IDM_FILE_PRINT_PREVIEW, _T("PreviewPrintout"));
	m_ContextHelp.AddHelpTopic(IDM_FILE_EXIT, _T("ExitTerminateProgram"));
	m_ContextHelp.AddHelpTopic(IDM_EDIT_UNDO, _T("UndoFunction"));
	m_ContextHelp.AddHelpTopic(IDM_EDIT_REDO, _T("RedoFunction"));
	m_ContextHelp.AddHelpTopic(IDM_EDIT_CUT, _T("CutFunction"));
	m_ContextHelp.AddHelpTopic(IDM_EDIT_COPY, _T("CopyFunction"));
	m_ContextHelp.AddHelpTopic(IDM_EDIT_PASTE, _T("PasteFunction"));
	m_ContextHelp.AddHelpTopic(IDM_EDIT_DELETE, _T("DeleteFunction"));
	m_ContextHelp.AddHelpTopic(IDM_EDIT_FIND, _T("FindInDocument"));
	m_ContextHelp.AddHelpTopic(IDM_EDIT_REPLACE, _T("ReplaceInDocument"));
	m_ContextHelp.AddHelpTopic(IDM_HELP_CONTENT, _T("Introduction"));
	m_ContextHelp.AddHelpTopic(IDM_HELP_CONTEXT, _T("Welcome"));
	m_ContextHelp.AddHelpTopic(IDM_HELP_ABOUT, _T("AboutThisProgram"));
	m_ContextHelp.AddHelpTopic(IDM_COLOR_CHOICE, _T("ColorChoiceFunction"));
	m_ContextHelp.AddHelpTopic(IDM_FONT_CHOICE, _T("FontChoiceFunction"));

	// standard menu topics
	m_ContextHelp.AddHelpTopic(IDW_VIEW_TOOLBAR, _T("ToolbarTopics"));
	m_ContextHelp.AddHelpTopic(IDW_VIEW_STATUSBAR, _T("StatusbarTopics"));
	m_ContextHelp.AddHelpTopic(IDW_ABOUT, _T("AboutThisProgram"));
}

void CAppHelp::SetClientHelpTopics()
{
	// define the help topics for the client area controls
	m_ContextHelp.AddHelpTopic(IDD_DIALOG1,        _T("clientwindowcontrols")); // dialog client area
	m_ContextHelp.AddHelpTopic(IDC_RICHEDIT1,	     _T("RichEditBoxUsage"));
	m_ContextHelp.AddHelpTopic(IDC_EDIT1,	    	 _T("EditBoxUsage"));
	m_ContextHelp.AddHelpTopic(ID_RADIO_A, 	     _T("RadioButtonUsage"));
	m_ContextHelp.AddHelpTopic(ID_RADIO_B, 	     _T("RadioButtonUsage"));
	m_ContextHelp.AddHelpTopic(ID_RADIO_C, 	     _T("RadioButtonUsage"));
	m_ContextHelp.AddHelpTopic(ID_CHECK_A, 	     _T("CheckBoxUsage"));
	m_ContextHelp.AddHelpTopic(ID_CHECK_B, 	     _T("CheckBoxUsage"));
	m_ContextHelp.AddHelpTopic(ID_CHECK_C, 	     _T("CheckBoxUsage"));
	m_ContextHelp.AddHelpTopic(IDC_BUTTON1, 	     _T("Button1Usage"));
	m_ContextHelp.AddHelpTopic(IDC_STATUS, 	     _T("StatusBoxContent"));
	m_ContextHelp.AddHelpTopic(IDC_BITMAP1, 	     _T("BitmapContent"));
	m_ContextHelp.AddHelpTopic(IDOK,	     	     _T("OKButtonUsage"));
	
	// define common dialog help button topics
	m_ContextHelp.AddHelpTopic(IDM_HELP_COLORDLG,      _T("ColorDialogHelp"));
	m_ContextHelp.AddHelpTopic(IDM_HELP_FILEDLG_OPEN,  _T("FileDialogOpenHelp"));
	m_ContextHelp.AddHelpTopic(IDM_HELP_FILEDLG_NEW,   _T("FileDialogNewHelp"));
	m_ContextHelp.AddHelpTopic(IDM_HELP_FILEDLG_SAVEAS, _T("FileDialogSaveAsHelp"));
	m_ContextHelp.AddHelpTopic(IDM_HELP_FONTDLG,       _T("FontChoiceDialogHelp"));
	m_ContextHelp.AddHelpTopic(IDM_HELP_PAGESETDLG,    _T("PageSetupDialogHelp"));
	m_ContextHelp.AddHelpTopic(IDM_HELP_PRINTDLG,      _T("PrinterDialogHelp"));
	m_ContextHelp.AddHelpTopic(IDM_HELP_FINDDLG,       _T("FindTextDialogHelp"));
	m_ContextHelp.AddHelpTopic(IDM_HELP_REPLACEDLG,    _T("ReplaceTextDialogHelp"));
	m_ContextHelp.AddHelpTopic(IDC_LIST1,   			_T("ListBoxDialogHelp"));
}

void CAppHelp::SetFrameHelpTopics()
{
	// define the context help topics for the frame
	m_ContextHelp.AddHelpTopic(IDFR_CLOSE,		_T("ExitTerminateProgram"));
	m_ContextHelp.AddHelpTopic(IDFR_SYSTEMMENU,	_T("ProgramIcon"));
	m_ContextHelp.AddHelpTopic(IDFR_CAPTION,		_T("AppTitleBar"));
	m_ContextHelp.AddHelpTopic(IDFR_GRIPPER, _T("ResizeGripper"));
}

void CAppHelp::SetHelpFilePath(LPCTSTR chmName)
{
	m_ContextHelp.SetHelpFilePath(chmName);
}

void CAppHelp::ShowHelpTopic(UINT nID)
// Display the context help for the specified topic
{
	m_ContextHelp.ShowHelpTopic(nID);
}

void CAppHelp::ShowHelpTopic(LPCTSTR topic)
// Display the context help for the specified topic
{
	m_ContextHelp.ShowHelpTopic(topic);
}

void CAppHelp::About()
{
	m_HelpAbout.DoModal();
}

void CAppHelp::SetCredits(LPCTSTR szCredits)
{
	m_HelpAbout.SetCredits(szCredits);
}
