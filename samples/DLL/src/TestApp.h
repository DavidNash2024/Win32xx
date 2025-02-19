/////////////////////////////
// TestApp.h
//

#ifndef TESTAPP_H
#define TESTAPP_H


class CTestApp : public CWinApp
{
public:
    CTestApp();
    virtual ~CTestApp();
    virtual BOOL InitInstance() override;

    void ShowDllDialog();

private:
    HMODULE m_module;
    CTestWindow m_testWindow;
};


#endif // TESTAPP_H
