/////////////////////////////
// Splash.h
//

#ifndef SPLASH_H
#define SPLASH_H

///////////////////////////////////////////////////////////////////////
// CSplash creates a splash screen with optional text and progress bar.
// The splash screen is shown to indicate time consuming tasks, and
// hidden when they're complete.
class CSplash : public CWnd
{
public:
    CSplash();
    ~CSplash();

    const CProgressBar& GetBar() const { return m_progress; }

    void AddBar();
    void Hide();
    void LoadFont();
    void RemoveBar();
    void ShowText(LPCTSTR text);

protected:
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual void OnDraw(CDC& dc);
    virtual void PreCreate(CREATESTRUCT& cs);
    virtual void PreRegisterClass(WNDCLASS& wc);

private:
    CProgressBar m_progress;
    CString      m_text;
    HICON        m_hIcon;
    HANDLE       m_fontHandle;
};


#endif // SPLASH_H
