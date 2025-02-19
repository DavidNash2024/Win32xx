/////////////////////////////
// MyEdit.h
//

#ifndef MYEDIT_H
#define MYEDIT_H


///////////////////////////////////////////////////////
// CMyEdit manages an edit control. The edit control is
// a child window of the main window.
class CMyEdit : public CEdit
{
public:
    CMyEdit() = default;
    virtual ~CMyEdit() override = default;

    void DPISetFont();

protected:
    // Virtual functions that override base class functions
    virtual void OnAttach() override;
    virtual void PreCreate(CREATESTRUCT& cs) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMyEdit(const CMyEdit&) = delete;
    CMyEdit& operator=(const CMyEdit&) = delete;

    CFont m_font;
};

#endif // MYEDIT_H
