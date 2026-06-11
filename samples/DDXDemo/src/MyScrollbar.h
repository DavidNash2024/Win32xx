/////////////////////////////////////////
// MyScrollbar.h
// Authors: Robert Tausworthe, David Nash
//


#ifndef MYSCROLLBAR_H
#define MYSCROLLBAR_H

/////////////////////////////////////////////////////////
// CMyScrollBar manages the scroll bar control in dialog.
//
class CMyScrollBar : public CScrollBar
{
    public:
        CMyScrollBar();
        virtual ~CMyScrollBar() override = default;

        int     GetHScrollPos(WPARAM, LPARAM);
        void    SetScrollInfo(int lo, int hi, int pos, int page);

    private:
        CMyScrollBar(const CMyScrollBar&) = delete;
        CMyScrollBar& operator=(const CMyScrollBar&) = delete;

        SCROLLINFO m_scrollInfo;
};

#endif  // MYSCROLLBAR_H
