/////////////////////////////
// ProgressBar.h
//

#ifndef MYPROGRESSBAR_H
#define MYPROGRESSBAR_H

//////////////////////////////////////////
// Declaration of the CMyProgressBar class
//
class CMyProgressBar : public CProgressBar
{
public:
    CMyProgressBar() {}
    virtual ~CMyProgressBar() {}

    void SetProgress(int nPos);
};


#endif
