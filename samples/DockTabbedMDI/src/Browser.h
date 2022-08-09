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

protected:
    // Virtual functions that override base class functions
    virtual int OnCreate(CREATESTRUCT& cs);

private:
    CViewWeb(const CViewWeb&);                // Disable copy construction
    CViewWeb& operator = (const CViewWeb&);   // Disable assignment operator
};


#endif // BROWSER_H
