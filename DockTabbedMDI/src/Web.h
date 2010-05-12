//////////////////////////////////////////////////
// Web.h - Declaration of the CViewBrowser class

#ifndef WEB_H
#define WEB_H



// Declaration of the CViewBrowser class
class CViewBrowser : public CWebBrowser
{
public:
	CViewBrowser() {}
	virtual ~CViewBrowser() {}

	virtual void OnInitialUpdate() 
	{
		GetIWebBrowser2()->GoHome();
	}

protected:
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

// Declaration of the CContainBrowser class
class CContainBrowser : public CDockContainer
{
public:
	CContainBrowser();
	~CContainBrowser() {}

private:
	CViewBrowser m_ViewWeb;
};

// Declaration of the CDockBrowser class
class CDockBrowser : public CDocker
{
public:
	CDockBrowser();
	virtual ~CDockBrowser() {}
	virtual void OnInitialUpdate();

private:
	CContainBrowser m_View;

};


#endif // WEB_H
