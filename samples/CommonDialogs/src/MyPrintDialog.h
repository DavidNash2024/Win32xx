/////////////////////////////////////////
// MyPrintDialog.h
// Authors: Robert Tausworthe, David Nash
//


#ifndef MYPRINTER_H
#define MYPRINTER_H

//////////////////////////////////////////////////////////////////////////
// This class extends the CPrintDialog class to add a title to the dialog.
class MyPrintDialog : public CPrintDialog
{
public:
    MyPrintDialog(
        DWORD dwFlags = PD_ALLPAGES |
        PD_USEDEVMODECOPIES |
        PD_NOPAGENUMS |
        PD_HIDEPRINTTOFILE |
        PD_NOSELECTION |
        PD_ENABLEPRINTHOOK)
        : CPrintDialog(dwFlags) {}
    virtual ~MyPrintDialog() override = default;

    void    SetBoxTitle (LPCWSTR title) {m_sPDTitle  = title;}

protected:
    virtual BOOL OnInitDialog() override;
    virtual void OnCancel() override;
    virtual void OnOK() override {}

private:
    MyPrintDialog(const MyPrintDialog&) = delete;
    MyPrintDialog& operator=(const MyPrintDialog&) = delete;

    CString m_sPDTitle;
};

class MyPageSetup : public CPageSetupDialog
{
public:
    MyPageSetup(DWORD dwFlags = PSD_MARGINS) : CPageSetupDialog(dwFlags) {}
    virtual ~MyPageSetup() override = default;

    void SetBoxTitle(LPCWSTR title);

protected:
    virtual BOOL OnInitDialog() override;
    virtual void OnCancel() override;
    virtual void OnOK() override {}

private:
    MyPageSetup(const MyPageSetup&) = delete;
    MyPageSetup& operator=(const MyPageSetup&) = delete;

    CString  m_PSDTitle;
};

#endif  // MYPRINTER_H
