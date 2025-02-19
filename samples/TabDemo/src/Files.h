/////////////////////////////
// Files.h
//

#ifndef FILES_H
#define FILES_H


//////////////////////////////////////////////////
// CViewDialog manages a list view control.
// It is used as a view window by the tab control.
class CViewFiles : public CListView
{
public:
    CViewFiles() = default;
    virtual ~CViewFiles() override;
    void InsertItems();
    void SetColumns();
    void SetDPIColumnWidths();
    void SetDPIImages();

protected:
    // Virtual functions that override base class functions
    virtual void OnAttach() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CViewFiles(const CViewFiles&) = delete;
    CViewFiles& operator=(const CViewFiles&) = delete;

    // Command handlers
    LRESULT OnDpiChangedBeforeParent(UINT msg, WPARAM wparam, LPARAM lparam);
};


#endif // FILES_H
