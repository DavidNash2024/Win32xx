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
    CMyTab() = default;
    virtual ~CMyTab() override = default;

protected:
    virtual void UpdateTabs() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMyTab(const CMyTab&) = delete;
    CMyTab& operator=(const CMyTab&) = delete;
};


#endif  // MYTAB_H