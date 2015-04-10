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
	BOOL OnMDICascade() { MDICascade(); return TRUE; }
	BOOL OnMDICloseAll() { RemoveAllMDIChildren(); return TRUE; }
	BOOL OnMDIIconArrange() { MDIIconArrange(); return TRUE; }
	BOOL OnMDITile() { MDITile(); return TRUE; }

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual int  OnCreate(LPCREATESTRUCT pcs);
	virtual void OnInitialUpdate();
	virtual void SetupToolBar();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif  //MAINMDIFRM_H

