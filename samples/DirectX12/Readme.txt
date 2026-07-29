DirectX12 Example
=================
This project displays a uses DirectX to display a rotating triangle in a simple
window. The code in this example is based on DirectX version 12.

The Visual Studio Community development environments include
the DirectX12 SDK, so no additional software is required to compiler this
code if VS Community is used.

DirectX12 is included as part of both Windows 10 and Windows 11.

Note: The Shader12.hlsl file is required for the compiled program to run.

Features demonstrated in this example
=====================================
* Displaying a moving DirectX picture in a window.
* Use of OnIdle to render the DirectX image when the application isn't
  processing window messages.
* Use of Microsoft::WRL::ComPtr to manage DirectX resources safely.


