//////////////////////////////////////////////////////
// TestWnd.h
//  Declaration of the CTestWindow class

#ifndef TESTWND_H
#define TESTWND_H


class CTestWindow : public CWnd, public CThread
{
public:
	CTestWindow(int nWindow);
	virtual ~CTestWindow() {}

protected:
	// Override CThread member functions
	virtual BOOL InitInstance();

	// Override CWnd member functions
	virtual HWND Create(CWnd* pWndParent = 0);
	virtual void OnClose();
	virtual void OnDestroy();
	virtual void OnInitialUpdate();

private:
	enum Constants
	{
		WM_WINDOWCREATED     = WM_USER+1,
		WM_TESTMESSAGE       = WM_USER+2
	};

	int m_nWindow;
};


#endif  //TESTWIN_H
