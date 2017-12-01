//////////////////////////////////////////////
// MyProgressBar.h

#ifndef MYPROGRESSBAR_H
#define MYPROGRESSBAR_H



class CMyProgressBar : public CProgressBar
{
public:
    CMyProgressBar() {}
    virtual ~CMyProgressBar() {}
    
    void SetProgress(int nPos);
};


#endif
