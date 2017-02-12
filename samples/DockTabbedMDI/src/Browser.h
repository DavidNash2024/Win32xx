//////////////////////////////////////////////////
// Browser.h - Declaration of the CViewWeb class

#ifndef BROWSER_H
#define BROWSER_H



// Declaration of the CViewWeb class
class CViewWeb : public CWebBrowser
{
public:
	CViewWeb() {}
	virtual ~CViewWeb() {}
	virtual int OnCreate(CREATESTRUCT& cs);

};


#endif // BROWSER_H
