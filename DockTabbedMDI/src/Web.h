//////////////////////////////////////////////////
// Web.h - Declaration of the CViewWeb class

#ifndef WEB_H
#define WEB_H



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

// Declaration of the CContainWeb class
class CContainWeb : public CContainer
{
public:
	CContainWeb();
	~CContainWeb() {}

private:
	CViewWeb m_ViewWeb;
};

// Declaration of the CDockWeb class
class CDockWeb : public CDocker
{
public:
	CDockWeb();
	virtual ~CDockWeb() {}
	virtual void OnInitialUpdate();

private:
	CContainWeb m_View;

};


#endif // WEB_H
