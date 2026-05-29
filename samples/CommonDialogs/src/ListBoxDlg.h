/////////////////////////////////////////
// ListBox.h
// Authors: Robert Tausworthe, David Nash
//


#ifndef CLISTVIEW_BOX_H_DEFINED
#define CLISTVIEW_BOX_H_DEFINED

////////////////////////////////////////////////////////////////////
// This class is displays a popup window containing a list box whose
// items may be selected.
class CListBoxDlg : public CDialog
{
public:
    CListBoxDlg();
    virtual ~CListBoxDlg() override = default;

    void   AddListItem(LPCWSTR s)       { m_listContent.push_back(s);}
    void    ClearList(){ m_listContent.clear();}
    void    SetBoxTitle(LPCWSTR title)  { m_boxTitle = title;}

protected:
    virtual INT_PTR DialogProc(UINT, WPARAM, LPARAM) override;
    virtual void    OnCancel() override;
    virtual BOOL    OnInitDialog() override;
    virtual void    OnOK() override;

private:
    CListBoxDlg(const CListBoxDlg&) = delete;
    CListBoxDlg& operator=(const CListBoxDlg&) = delete;

    CListBox    m_listBox;              // The IDC_LIST_BOX control.
    CString     m_boxTitle;             // The list box title.
    std::vector<CString> m_listContent; // The list strings.
};

#endif // CLISTVIEW_BOX_H_DEFINED
