/* (26-Mar-2025)                                          (MyFindReplaceDlg.h) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Declaration and implementation of the MyFindReplaceDlg
    class. This class is derived from the CFindREplaceDalog class found in the
    Win32++ Windows interface framework in order to add a title to the dialog.

*******************************************************************************/

#ifndef MY_FIND_REPLACE_DLG_H
#define MY_FIND_REPLACE_DLG_H

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
