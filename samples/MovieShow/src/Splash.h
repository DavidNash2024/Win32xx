#ifndef SPLASH_H
#define SPLASH_H

// This class creates a splash screen with optional text and progress bar.
// Add a CSplash object to any time consuming function.
// The constructor automatically creates the splash screen window.
// The splash screen window will be automatically destroyed when the
//  CSplash object goes out of scope.
class CSplash : public CWnd
{
public:
    CSplash();
    CSplash(LPCTSTR text);
    ~CSplash();

    void CreateBar();
    CProgressBar& GetBar() { return m_progress; }
    void LoadFont();
    void SetText(LPCTSTR text) { m_text = text; }

protected:
    virtual int OnCreate(CREATESTRUCT& cs);
    virtual void OnDraw(CDC& dc);

    virtual void PreCreate(CREATESTRUCT& cs);
    virtual void PreRegisterClass(WNDCLASS& wc);

private:
    CProgressBar m_progress;
    CString m_text;
    HICON m_hIcon;
    HANDLE m_fontHandle;
};


#endif // SPLASH_H
