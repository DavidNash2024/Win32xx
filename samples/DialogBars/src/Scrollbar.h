/////////////////////////////
// ScrollBar.h
//

#ifndef MYSCROLLBAR_H
#define MYSCROLLBAR_H


//////////////////////////////////////////////
// CMyScrollBar manages the scrollbar control.
class CMyScrollBar : public CScrollBar
{
public:
    CMyScrollBar();
    virtual ~CMyScrollBar() {}

    int  GetPos(WPARAM wparam);
    void SetPos(int pos);

protected:
    // Virtual functions that override base class functions
    virtual void OnInitialUpdate();

private:
    SCROLLINFO m_si;
};


#endif
