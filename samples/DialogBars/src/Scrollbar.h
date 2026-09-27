/////////////////////////////
// ScrollBar.h
//

#pragma once

//////////////////////////////////////////////
// CMyScrollBar manages the scrollbar control.
class CMyScrollBar : public CScrollBar
{
public:
    CMyScrollBar();
    virtual ~CMyScrollBar() override = default;

    int  GetPos(WPARAM wparam);
    void SetPos(int pos);

protected:
    virtual void OnInitialUpdate() override;

private:
    CMyScrollBar(const CMyScrollBar&) = delete;
    CMyScrollBar& operator=(const CMyScrollBar&) = delete;

    SCROLLINFO m_si;
};

