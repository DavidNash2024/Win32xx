//////////////////////////////////////////////////
// Simple.h - Declaration of the CViewWeb class

#ifndef SIMPLE_H
#define SIMPLE_H



// Declaration of the CViewWeb class
class CViewWeb : public CWebBrowser
{
public:
	CViewWeb() {}
	virtual ~CViewWeb() {}

	virtual void OnInitialUpdate() 
	{
		GetIWebBrowser2()->GoHome();
	}

protected:
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
};


#endif // SIMPLE_H
