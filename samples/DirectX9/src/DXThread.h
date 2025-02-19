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
    CDXThread();
    virtual ~CDXThread() override = default;

protected:
    // Virtual functions that override base class functions
    virtual BOOL InitInstance() override;
    virtual int MessageLoop() override;

private:
    CDXThread(const CDXThread&) = delete;
    CDXThread& operator=(const CDXThread&) = delete;

    CDXView* m_pView;
};

#endif
