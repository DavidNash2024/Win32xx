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
	CMainMDIFrame();
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
	BOOL OnMDICascade();
	BOOL OnMDICloseAll();
	BOOL OnMDIIconArrange();
	BOOL OnMDITile();

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual int  OnCreate(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	virtual void SetupMenuIcons();
	virtual void SetupToolBar();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif  //MAINMDIFRM_H

