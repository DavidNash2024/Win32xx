/////////////////////////////////////////
// MyFindReplaceDlg.h
// Authors: Robert Tausworthe, David Nash
//


#ifndef MY_FIND_REPLACE_DLG_H
#define MY_FIND_REPLACE_DLG_H

///////////////////////////////////////////////////////////////////
// MyFindReplaceDialog inherits from the Win32++ CFindReplaceDialog
// class.
class MyFindReplaceDialog : public CFindReplaceDialog
{
    public:
        MyFindReplaceDialog() = default;
        virtual ~MyFindReplaceDialog() override = default;

        void SetBoxTitle(LPCWSTR title);

    protected:
        virtual BOOL OnInitDialog() override;

    private:
        MyFindReplaceDialog(const MyFindReplaceDialog&) = delete;
        MyFindReplaceDialog& operator=(const MyFindReplaceDialog&) = delete;

        CString m_boxTitle;
};

#endif // MY_FIND_REPLACE_DLG_H
