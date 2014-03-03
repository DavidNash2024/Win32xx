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

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual int  OnCreate(LPCREATESTRUCT pcs);
	virtual void OnFileClose();
	virtual void OnFileExit();
	virtual void OnFileNew();
	virtual void OnFileNewList();
	virtual void OnFileNewMax();
	virtual void OnFileNewRect();
	virtual void OnFileNewText();
	virtual void OnFileNewTree();
	virtual void OnFileNewView();
	virtual void OnInitialUpdate();
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	virtual void SetupToolBar();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif  //MAINMDIFRM_H

