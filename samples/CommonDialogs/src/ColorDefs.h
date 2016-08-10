/* (26-Feb-2000) [Tab/Indent: 8/8][Line/Box: 80/74]              (ColorDefs.h) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2000, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
===============================================================================*

	Content Description: This file contains a number of commonly used
	RGB Color Definitions.

        Caveats: The copyright displayed above extends only to the author's
	original contributions to the subject class, and to the alterations,
	additions, deletions, and other treatments of materials that may have
	been extracted from cited sources. Unaltered portions of those
	materials retain their original copyright status. The author hereby
	grants permission to any person obtaining a copy of this treatment
	of the subject class and any associated documentation composed by
	the author, to utilize this material, free of charge and without
	restriction or limitation, subject to the following conditions:

		The above copyright notice, together with the respective
		permission conditions shall be included in all copies or
		substantial portions of this material so copied, modified,
		merged, published, distributed, or otherwise held by others.

	These materials are provided "as is", without warranty of any kind,
	express or implied, including but not limited to: warranties of
	merchantability, fitness for a particular purpose, and non-infringement.
	In no event shall the authors or copyright holders be liable for any
	claim, damages, or other liability, whether in an action of contract,
	tort or otherwise, arising from, out of, or in connection with, these
	materials, the use thereof, or any other other dealings therewith.

 	Acknowledgement:
		The author would like to thank and acknowledge the advice,
		critical review, insight, and assistance provided by David Nash
		in the development of this work.

	Programming Notes:
               The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
		Planning and Preparation Subsystem project for C++ programming.

********************************************************************************

	Declaration of Color RGB definitions

*******************************************************************************/

#ifndef COLORDEFS_IS_LOADED
#define COLORDEFS_IS_LOADED

/*============================================================================*/

#define COLOR_BLACK		RGB(  0,   0,   0)
#define COLOR_GRAY		RGB(100, 100, 100)
#define COLOR_LT_GRAY		RGB(192, 192, 192)
#define COLOR_WHITE		RGB(255, 255, 255)

#define COLOR_BLUE		RGB(  0,   0, 255)
#define COLOR_LT_BLUE		RGB(128, 128, 255)
#define COLOR_DK_BLUE		RGB(  0,   0, 128)

#define COLOR_BROWN		RGB(255, 128,  64)
#define COLOR_DK_BROWN		RGB(128,  64,   0)

#define COLOR_YELLOW		RGB(255, 255,   0)
#define COLOR_LT_YELLOW         RGB(255, 255, 128)
#define COLOR_DK_YELLOW		RGB(128, 128,   0)

#define COLOR_CYAN		RGB(  0, 255, 255)
#define COLOR_LT_CYAN		RGB(128, 255, 255)
#define COLOR_DK_CYAN		RGB(  0, 128, 128)

#define COLOR_GREEN		RGB(  0, 255,   0)
#define COLOR_LT_GREEN		RGB(128, 255, 128)
#define COLOR_DK_GREEN		RGB(  0, 128,   0)

#define COLOR_MAGENTA		RGB(255,   0, 255)
#define COLOR_LT_MAGENTA	RGB(255, 128, 255)
#define COLOR_DK_MAGENTA	RGB(128,   0, 128)

#define COLOR_RED		RGB(255,   0,   0)
#define COLOR_LT_RED		RGB(255, 128, 128)
#define COLOR_DK_RED		RGB(128,   0,   0)

/*----------------------------------------------------------------------------*/
#endif /* COLORDEFS_IS_LOADED */
