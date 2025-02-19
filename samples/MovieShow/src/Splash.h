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
    virtual ~CSplash() override = default;
    CSplash(const CSplash&) = delete;
    CSplash& operator=(const CSplash&) = delete;

    const CProgressBar& GetBar() const { return m_progress; }

    void AddBar();
    void Hide();
    void ShowText(LPCWSTR text, CWnd* parent);

protected:
    // Virtual functions that override base class functions
    virtual int  OnCreate(CREATESTRUCT& cs) override;
    virtual void OnDraw(CDC& dc) override;
    virtual void PreCreate(CREATESTRUCT& cs) override;
    virtual void PreRegisterClass(WNDCLASS& wc) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    void LoadFont();
    void RemoveBar();

    CProgressBar m_progress;
    CString      m_text;
    HICON        m_hIcon;
    HANDLE       m_fontHandle;
};


#endif // SPLASH_H
