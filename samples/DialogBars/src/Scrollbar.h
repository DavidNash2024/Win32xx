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
    CMyScrollBar(const CMyScrollBar&);                // Disable copy construction
    CMyScrollBar& operator = (const CMyScrollBar&);   // Disable assignment operator

    SCROLLINFO m_si;
};


#endif
