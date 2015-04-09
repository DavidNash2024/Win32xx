///////////////////////////////////////////////////////
// Mainfrm.h
//  Declaration of the CMainFrame class

#ifndef MAINFRM_H
#define MAINFRM_H

#include "targetver.h"
#include "frame.h"
#include "file.h"
#include "View.h"

// required for Dev-C++
#ifndef OPENFILENAME_SIZE_VERSION_400
#define OPENFILENAME_SIZE_VERSION_400 sizeof(OPENFILENAME)
#endif

class CMainFrame : public CFrame
{
public:
	CMainFrame(void);
	virtual ~CMainFrame();

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnFileExit();
	virtual void OnFileNew();
	virtual void OnFileOpen();
	virtual void OnFileSave();
	virtual void OnFileSaveAs();
	virtual void OnFilePrint();
	virtual void OnPenColor();
	virtual void SetupToolBar();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CView m_View;
	CString m_PathName;

};

#endif //MAINFRM_H

