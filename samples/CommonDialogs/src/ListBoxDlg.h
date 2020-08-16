/* (28-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]              (ListBoxDlg.h) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of the CListBoxDlg subclass of the common
    dialog class for this sample program based on the Win32++ Windows interface 
    classes. This class is displays a popup window containing a list box whose 
    items may be selected.

    Programming Notes: The programming standards roughly follow those 
    established by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
    Planning and Preparation Subsystem project for C++ programming.

*******************************************************************************/

#ifndef CLISTVIEW_BOX_H_DEFINED
#define CLISTVIEW_BOX_H_DEFINED

/*============================================================================*/
        class
CListBoxDlg : public CDialog                                                  /*

*-----------------------------------------------------------------------------*/
{
    public:
        CListBoxDlg();
        virtual ~CListBoxDlg(){}

        virtual  void   AddListItem(LPCTSTR s)
                          { m_listContent.push_back(s);}
        virtual void    ClearList(){ m_listContent.clear();}
        virtual void    OnCancel();
        virtual void    OnOK();
        virtual void    SetBoxTitle(LPCTSTR title)
                          { m_boxTitle = title;}

    protected:
        virtual INT_PTR DialogProc(UINT, WPARAM, LPARAM);
        virtual BOOL    OnInitDialog();
        virtual void    SetWindowTitle() const
                          { SetWindowText(m_boxTitle);}

        CListBox    m_listBox;      // the IDC_LIST_BOX control
        CString     m_boxTitle;     // the list box title
        std::vector<CString> m_listContent; // the list strings
};
/*----------------------------------------------------------------------------*/
#endif // CLISTVIEW_BOX_H_DEFINED
