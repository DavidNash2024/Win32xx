///////////////////////////////////////////////////////
// MainMDIfrm.h
//  Declaration of the CMainFrame class

#ifndef MAINMDIFRM_H
#define MAINMDIFRM_H


// Define BTNS_WHOLEDROPDOWN
#ifndef BTNS_WHOLEDROPDOWN
  #define BTNS_WHOLEDROPDOWN 0x0080
#endif


class CMainMDIFrame : public CMDIFrame
{
public:
	CMainMDIFrame(void);
	virtual ~CMainMDIFrame();

	BOOL OnFileClose();
	BOOL OnFileExit();
	BOOL OnFileNew();
	BOOL OnFileNewList();
	BOOL OnFileNewMax();
	BOOL OnFileNewRect();
	BOOL OnFileNewText();
	BOOL OnFileNewTree();
	BOOL OnFileNewView();
	BOOL OnMDICascade() { MDICascade(); return TRUE; }
	BOOL OnMDICloseAll() { RemoveAllMDIChildren(); return TRUE; }
	BOOL OnMDIIconArrange() { MDIIconArrange(); return TRUE; }
	BOOL OnMDITile() { MDITile(); return TRUE; }

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual int  OnCreate(LPCREATESTRUCT pcs);
	virtual void OnInitialUpdate();
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	virtual void SetupToolBar();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif  //MAINMDIFRM_H

