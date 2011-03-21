Threads Example
==============
This project demonstrates the ability of Win32++ to create windows in
different threads. Each thread containing a window runs a message loop.

The CTestWindow class inherits from both CWnd and CThread. When the class is
instanciated, a new thread is created in the suspended state. When the thread
is resumed, it runs the InitInstance function which creates the window.


Features demonstrated in this example
=====================================
* How to create a thread that is initially suspended
* How to create windows in different threads
* Using PostMessage to communicate information between threads
* Using PostQuitMessage to end a GUI thread
* Using the CThread class
