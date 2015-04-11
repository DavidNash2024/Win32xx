///////////////////////////////////////////////////////
// MainMDIfrm.h
//  Declaration of the CMainFrame class

#ifndef MAINMDIFRM_H
#define MAINMDIFRM_H



// MDI frames are inherrited from CMDIFrame
class CMainMDIFrame : public CMDIFrame
{
public:
	CMainMDIFrame(void);
	virtual ~CMainMDIFrame();

	BOOL OnFileClose();
	BOOL OnFileExit();
	BOOL OnFileNew();
	BOOL OnFileOpen();
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

