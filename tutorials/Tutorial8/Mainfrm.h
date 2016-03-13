///////////////////////////////////////////////////////
// Mainfrm.h
//  Declaration of the CMainFrame class

#ifndef MAINFRM_H
#define MAINFRM_H

#include "targetver.h"
#include "wxx_frame.h"
#include "wxx_file.h"
#include "View.h"
#include "Doc.h"

// required for Dev-C++
#ifndef OPENFILENAME_SIZE_VERSION_400
#define OPENFILENAME_SIZE_VERSION_400 sizeof(OPENFILENAME)
#endif

class CMainFrame : public CFrame
{
public:
	CMainFrame(void);
	virtual ~CMainFrame();

	CDoc& GetDoc() { return m_Doc; }
	void  LoadFile(LPCTSTR str);

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnFileExit();
	virtual BOOL OnFileNew();
	virtual BOOL OnFileOpen();
	virtual BOOL OnFileSave();
	virtual BOOL OnFileSaveAs();
	virtual BOOL OnPenColor();
	virtual void SetupToolBar();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CView m_View;
	CDoc m_Doc;
	CString m_PathName;

};

#endif //MAINFRM_H

