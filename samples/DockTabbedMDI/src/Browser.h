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
    CViewWeb() = default;
    virtual ~CViewWeb() override = default;

protected:
    virtual int  OnCreate(CREATESTRUCT& cs) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CViewWeb(const CViewWeb&) = delete;
    CViewWeb& operator=(const CViewWeb&) = delete;
};


#endif // BROWSER_H
