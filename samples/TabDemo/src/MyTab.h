/////////////////////////////
// MyTab.h
//

#ifndef MYTAB_H
#define MYTAB_H


////////////////////////////////////////////////////
// CMyTab is used as the view window by CMainFrame.
//
class CMyTab : public CTab
{
public:
    CMyTab() {}
    virtual ~CMyTab() {}
    virtual void UpdateTabs();

protected:
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CMyTab(const CMyTab&);              // Disable copy construction
    CMyTab& operator=(const CMyTab&); // Disable assignment operator
};


#endif  // MYTAB_H