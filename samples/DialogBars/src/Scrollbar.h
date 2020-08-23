/////////////////////////////
// ScrollBar.h
//

#ifndef MYSCROLLBAR_H
#define MYSCROLLBAR_H

////////////////////////////////////////
// Declaration of the CMyScrollBar class
//
class CMyScrollBar : public CScrollBar
{
public:
    CMyScrollBar();
    virtual ~CMyScrollBar() {}

    int GetPos(WPARAM wparam);
    void SetPos(int pos);

protected:
    virtual void OnInitialUpdate();

private:
    SCROLLINFO m_si;
};


#endif
