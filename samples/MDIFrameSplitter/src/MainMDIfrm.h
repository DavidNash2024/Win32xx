///////////////////////////////////////////////////////
// MainMDIfrm.h
//  Declaration of the CMainFrame class

#ifndef MAINMDIFRM_H
#define MAINMDIFRM_H



// MDI frames are inherited from CMDIFrame
class CMainMDIFrame : public CMDIFrame
{
public:
	CMainMDIFrame();
	virtual ~CMainMDIFrame();

	BOOL OnFileClose();
	BOOL OnFileExit();
	BOOL OnFileOpen();
	BOOL OnFileNew();
	BOOL OnFileSave();
	BOOL OnFilePrint();
	BOOL OnMDICascade();
	BOOL OnMDICloseAll();
	BOOL OnMDIIconArrange();
	BOOL OnMDITile();

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual int  OnCreate(LPCREATESTRUCT pcs);
	virtual void OnInitialUpdate();
	virtual void SetupToolBar();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif  //MAINMDIFRM_H

