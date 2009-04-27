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
	CView& GetMyView() {return m_MyView;}
	void ModifyBitmap(int cRed, int cGreen, int cBlue);

protected:
	virtual void OnAdjustImage();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnCreate();
	virtual void OnFileOpen();
	virtual void OnInitialUpdate();
	virtual void SetupToolbars();
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CView m_MyView;
	tString m_PathName;

};

#endif //MAINFRM_H

