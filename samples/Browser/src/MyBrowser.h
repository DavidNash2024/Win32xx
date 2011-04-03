#ifndef MYBROWSER_H
#define MYBROWSER_H


//////////////////////////////////////////////////////////////
// CMyBrowser extends the capabilities provided by CWebBrowser
//  by adding a Status bar, and support for an Event Sink.
class CMyBrowser : public CWebBrowser
{
public:
	CMyBrowser() {}
	virtual ~CMyBrowser() {}

protected:
	virtual void OnCreate();

private:


};

#endif // MYBROWSER_H
