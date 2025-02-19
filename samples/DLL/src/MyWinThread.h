/////////////////////////////
// MyWinThread.h
//

#ifndef MYWINTHREAD_H
#define MYWINTHREAD_H

////////////////////////////////////////////////////
// CMyWinThread manages the threads used in the dll.
// CMyWinThread creates a dialog when it starts.
class CMyWinThread : public CWinThread
{
public:
    CMyWinThread();
    virtual ~CMyWinThread() override = default;

protected:
    // Virtual functions that override base class functions.
    virtual BOOL InitInstance() override;

private:
    CMyWinThread(const CMyWinThread&) = delete;
    CMyWinThread& operator=(const CMyWinThread&) = delete;

    CMyDialog m_myDialog;
};

#endif  // MYWINTHREAD_H