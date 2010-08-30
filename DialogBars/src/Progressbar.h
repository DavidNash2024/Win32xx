//////////////////////////////////////////////
// MyProgressbar.h

#ifndef MYPROGRESSBAR_H
#define MYPROGRESSBAR_H



class CMyProgressbar : public CProgressbar
{
public:
	CMyProgressbar() {}
	virtual ~CMyProgressbar() {}
	
	void SetProgress(int nPos);
};


#endif
