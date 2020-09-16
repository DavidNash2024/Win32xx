/////////////////////////////
// Browser.h
//

#ifndef BROWSER_H
#define BROWSER_H


//////////////////////////////////////////////////////
// CViewWeb manages a window that displays a web page.
// This a view window used by CMyTabbedMDI.
class CViewWeb : public CWebBrowser
{
public:
    CViewWeb() {}
    virtual ~CViewWeb() {}
    virtual int OnCreate(CREATESTRUCT& cs);

};


#endif // BROWSER_H
