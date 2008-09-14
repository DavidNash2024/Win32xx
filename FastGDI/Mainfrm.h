/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H

#include "../Win32++/frame.h"
#include "View.h"
#include "ColourDialog.h"

// required for Dev-C++
#ifndef OPENFILENAME_SIZE_VERSION_400
  #define OPENFILENAME_SIZE_VERSION_400 sizeof(OPENFILENAME)
#endif


// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
	CMainFrame(void);
	virtual ~CMainFrame();
	CView& GetMyView() {return m_View;}

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnCreate();
	virtual void OnFileOpen();
	virtual void OnInitialUpdate();
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CView m_View;
	tString m_PathName;

};

#endif //MAINFRM_H

