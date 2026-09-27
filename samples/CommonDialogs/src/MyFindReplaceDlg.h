/////////////////////////////////////////
// MyFindReplaceDlg.h
// Authors: Robert Tausworthe, David Nash
//

#pragma once

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

