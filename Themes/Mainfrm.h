/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H

#include "../Win32++/frame.h"
#include "View.h"
#include "ComboBoxEx.h"


// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
	CMainFrame(void);
	virtual ~CMainFrame();
	void AddCombo();
	void ChooseTheme(UINT nStyle);

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnCreate();
	virtual void OnInitialUpdate();
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	virtual void LoadToolbar();
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CView m_View;
	CToolbar Arrows;
	CToolbar Cards;
	CComboBoxEx m_ComboBoxEx;
};

#endif //MAINFRM_H

