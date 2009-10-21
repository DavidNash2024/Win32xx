//////////////////////////////////////////////
// Progressbar.h

#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H



class CProgressbar : public CWnd
{
public:
	CProgressbar() {}
	virtual ~CProgressbar() {}
	
	void SetProgress(int nPos);
};


#endif
