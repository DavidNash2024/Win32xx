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
    CViewFiles();
    virtual ~CViewFiles();
    void InsertItems();
    void SetColumns();
    void SetDPIColumnWidths();
    void SetDPIImages();

protected:
    // Virtual functions that override base class functions
    virtual void OnAttach();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CViewFiles(const CViewFiles&);                // Disable copy construction
    CViewFiles& operator = (const CViewFiles&);   // Disable assignment operator

    CImageList m_smallImages;

    // Command handlers
    LRESULT OnUserDPIChanged(UINT msg, WPARAM wparam, LPARAM lparam);
};


#endif // FILES_H
