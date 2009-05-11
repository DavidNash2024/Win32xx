#include "MyTabbedMDI.h"
#include "Mainfrm.h"

CWnd* CMyTabbedMDI::NewMDIChildFromID(int nID)
{
	CWnd* pView = NULL;
	switch(nID)
	{
	case ID_CLASSES1:
		pView = new CViewClasses;
		break;
	case ID_FILES1:
		pView = new CViewFiles;
		break;
	case ID_OUTPUT1:
		pView = new CViewOutput;
		break;
	case ID_TEXT1:
		pView = new CViewText;
		break;
	case ID_SIMPLE:
		pView = new CViewSimple;
		break;
	case ID_RECT:
		pView = new CViewRect;
		break;
	default:
		TRACE(_T("Unknown TabbedMDI ID\n"));
		break;
	}

	return pView;
}