DirectX11 Example
=================
This project displays a uses DirectX to display a rotating cube in a simple
window. The code in this example is based on DirectX version 11.

The Visual Studio Community development environments include
the DirectX11 SDK, so no additional software is required to compiler this
code if VS Community is used.

DirectX11 is supported on Windows Vista Service Pack 2 and later operating
systems. The DirectX runtime is normally delivered via Windows Update.

Note: The DirectX11.fx shader file is required for the compiled program to run.

Features demonstrated in this example
=====================================
* Displaying a moving DirectX picture in a window.
* Use of OnIdle to render the DirectX image when the application isn't
  processing window messages.
* Use of Microsoft::WRL::ComPtr to manage DirectX resources safely.


