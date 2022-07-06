/////////////////////////////
// DXThread.h
//

#ifndef DXTHREAD_H
#define DXTHREAD_H


// Forward declaration of the CDXView class.
class CDXView;

/////////////////////////////////////////
// CDXThread runs the thread for CDXView
//
class CDXThread : public CWinThread
{
public:
    CDXThread() : m_pView(0) {}
    virtual ~CDXThread() {}

protected:
    // Virtual functions that override base class functions
    virtual BOOL InitInstance();
    virtual int MessageLoop();

private:
    CDXView* m_pView;
};

#endif
