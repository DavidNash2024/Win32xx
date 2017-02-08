
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
	AddHelpTopic(IDW_MENUBAR, _T("ToolbarandMenuItems"));
	AddHelpTopic(IDW_STATUSBAR, _T("StatusbarTopics"));
	AddHelpTopic(IDM_FILE_NEW, _T("NewDocument"));
	AddHelpTopic(IDM_FILE_OPEN, _T("OpenExistingDocument"));
	AddHelpTopic(IDM_FILE_SAVE, _T("SaveCurrentDocument"));
	AddHelpTopic(IDM_FILE_SAVEAS, _T("SaveAsAnotherDocument"));
	AddHelpTopic(IDM_FILE_CLOSE, _T("CloseCurrentDocument"));
	AddHelpTopic(IDM_FILE_PRINT, _T("PrintDocument"));
	AddHelpTopic(IDM_FILE_PRINT_PREVIEW, _T("PreviewPrintout"));
	AddHelpTopic(IDM_FILE_EXIT, _T("ExitTerminateProgram"));
	AddHelpTopic(IDM_EDIT_UNDO, _T("UndoFunction"));
	AddHelpTopic(IDM_EDIT_REDO, _T("RedoFunction"));
	AddHelpTopic(IDM_EDIT_CUT, _T("CutFunction"));
	AddHelpTopic(IDM_EDIT_COPY, _T("CopyFunction"));
	AddHelpTopic(IDM_EDIT_PASTE, _T("PasteFunction"));
	AddHelpTopic(IDM_EDIT_DELETE, _T("DeleteFunction"));
	AddHelpTopic(IDM_EDIT_FIND, _T("FindInDocument"));
	AddHelpTopic(IDM_EDIT_REPLACE, _T("ReplaceInDocument"));
	AddHelpTopic(IDM_HELP_CONTENT, _T("Introduction"));
	AddHelpTopic(IDM_HELP_CONTEXT, _T("Welcome"));
	AddHelpTopic(IDM_HELP_ABOUT, _T("AboutThisProgram"));
	AddHelpTopic(IDM_COLOR_CHOICE, _T("ColorChoiceFunction"));
	AddHelpTopic(IDM_FONT_CHOICE, _T("FontChoiceFunction"));
	AddHelpTopic(IDW_ABOUT, _T("AboutThisProgram"));
}

void CAppHelp::SetClientHelpTopics()
{
	// define the help topics for the client area controls
	AddHelpTopic(IDD_DIALOG1,        _T("clientwindowcontrols")); // dialog client area
	AddHelpTopic(IDC_RICHEDIT1,	     _T("RichEditBoxUsage"));
	AddHelpTopic(IDC_EDIT1,	    	 _T("EditBoxUsage"));
	AddHelpTopic(ID_RADIO_A, 	     _T("RadioButtonUsage"));
	AddHelpTopic(ID_RADIO_B, 	     _T("RadioButtonUsage"));
	AddHelpTopic(ID_RADIO_C, 	     _T("RadioButtonUsage"));
	AddHelpTopic(ID_CHECK_A, 	     _T("CheckBoxUsage"));
	AddHelpTopic(ID_CHECK_B, 	     _T("CheckBoxUsage"));
	AddHelpTopic(ID_CHECK_C, 	     _T("CheckBoxUsage"));
	AddHelpTopic(IDC_BUTTON1, 	     _T("Button1Usage"));
	AddHelpTopic(IDC_STATUS, 	     _T("StatusBoxContent"));
	AddHelpTopic(IDC_BITMAP1, 	     _T("BitmapContent"));
	AddHelpTopic(IDOK,	     	     _T("OKButtonUsage"));
	
	// define common dialog help button topics
	AddHelpTopic(IDM_HELP_COLORDLG,      _T("ColorDialogHelp"));
	AddHelpTopic(IDM_HELP_FILEDLG_OPEN,  _T("FileDialogOpenHelp"));
	AddHelpTopic(IDM_HELP_FILEDLG_NEW,   _T("FileDialogNewHelp"));
	AddHelpTopic(IDM_HELP_FILEDLG_SAVEAS, _T("FileDialogSaveAsHelp"));
	AddHelpTopic(IDM_HELP_FONTDLG,       _T("FontChoiceDialogHelp"));
	AddHelpTopic(IDM_HELP_PAGESETDLG,    _T("PageSetupDialogHelp"));
	AddHelpTopic(IDM_HELP_PRINTDLG,      _T("PrinterDialogHelp"));
	AddHelpTopic(IDM_HELP_FINDDLG,       _T("FindTextDialogHelp"));
	AddHelpTopic(IDM_HELP_REPLACEDLG,    _T("ReplaceTextDialogHelp"));
	AddHelpTopic(IDC_LIST1,   			_T("ListBoxDialogHelp"));
}

void CAppHelp::SetFrameHelpTopics()
{
	// define the context help topics for the frame
	AddHelpTopic(IDFR_CLOSE,		_T("ExitTerminateProgram"));
	AddHelpTopic(IDFR_SYSTEMMENU,	_T("SystemMenu"));
	AddHelpTopic(IDFR_CAPTION,		_T("AppTitleBar"));
	AddHelpTopic(IDFR_GRIPPER,		_T("ResizeGripper"));
	AddHelpTopic(IDFR_HELP,			_T("TitleBarQuestionBox"));
}
