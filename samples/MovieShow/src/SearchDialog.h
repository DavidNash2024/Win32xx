/////////////////////////////
// SearchDialog.h
//

#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H


/////////////////////////////////////////////////////////
// CSearchDialog allows the user to perform a word search
// for the movie's title, actors or description.
class CSearchDialog : public CDialog
{
public:
    CSearchDialog(int resID);
    virtual ~CSearchDialog();

    const CString& GetTitleString() { return m_titleText; }
    const CString& GetActorsString() { return m_actorsText; }
    const CString& GetInfoString() { return m_infoText; }

protected:
    // Virtual functions that override base class functions
    virtual BOOL OnInitDialog();
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual void OnOK();

private:
    CSearchDialog(const CSearchDialog&);                // Disable copy construction
    CSearchDialog& operator = (const CSearchDialog&);   // Disable assignment operator

    CButton m_titleButton;
    CButton m_actorsButton;
    CButton m_infoButton;

    CEdit   m_titleWindow;
    CEdit   m_actorsWindow;
    CEdit   m_infoWindow;

    CString m_titleText;
    CString m_actorsText;
    CString m_infoText;
};


#endif //SEARCHDIALOG_H
