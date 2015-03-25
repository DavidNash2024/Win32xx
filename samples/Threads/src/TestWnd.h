//////////////////////////////////////////////////////
// TestWnd.h
//  Declaration of the CTestWindow class

#ifndef TESTWND_H
#define TESTWND_H


class CTestWindow : public CWnd
{
public:
	CTestWindow(int nWindow);
	virtual ~CTestWindow() {}

	virtual HWND Create(HWND hWndParent = 0);

protected:
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
