Double Buffer Demo
==================
This sample demonstrates double buffering. Two moving balls are drawn to 
a memory DC. The second ball drawn is drawn with a mask so it neatly 
overlaps first ball.


Note: GDI drawing is not particularly fast. If you require faster graphics
rendering for animations, consider using GDIPlus or DirectX.


Features demonstrated in this example
=====================================
* Using CMemDC to create a memory DC for double buffering
* Creating a mask bitmap
* Using the mask bitmap to clip a bitmap image.