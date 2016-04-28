/* (02-Aug-2014) [Tab/Indent: 8/8][Line/Box: 80/74]               (StdAfx.cpp) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2015, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                                                                              |
===============================================================================*

	Contents Description: Standard declarations implementation File. This
	file is used for precompiled headers. See the details in stdafx.h.
	
        Caveats: The copyright displayed above extends only to the author's
	original contributions to the subject class, and  to the alterations,
	additions, deletions, and  other treatments of materials that may have
	been extracted from the cited sources.  Unaltered portions of those
	materials retain their original copyright status. The author hereby
	grants permission to any person obtaining a copy of this treatment
	of the subject class and  any associated documentation composed by
	the author, to utilize this material, free of charge and  without
	restriction or limitation, subject to the following conditions:

		The above copyright notice, as well as that of David Nash
		and Win32++, together with the respective permission
		conditions shall be included in all copies or substantial
		portions of this material so copied, modified, merged,
		published, distributed, or otherwise held by others.

	These materials are provided "as is", without warranty of any kind,
	express or implied, including but not limited to: warranties of
	merchantability, fitness for a particular purpose, and non-infringement.
	In no event shall the authors or copyright holders be liable for any
	claim, damages, or other liability, whether in an action of contract,
	tort or otherwise, arising from, out of, or in connection with, these
	materials, the use thereof, or any other other dealings therewith.

	To use precompiled headers, do the following:
	1) Add the set of rarely changed headers to stdafx.h
	2) Include stdafx.h in each cpp file. It must be included first.
	3) Compile stdafx.h into the precompiled hearder stdafx.h.gch by
	   including stdafx.mak into the makefile in the project options:

		all-before: stdafx.h.gch
                stdafx.h.gch: stdafx.h
			$(CPP) -c stdafx.h -o stdafx.h.gch $(CXXFLAGS)

	4) Add stdafx.cpp to your project source files.

	Special Conventions:

	Programming Notes:
                The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
		Planning and Preparation Subsystem project for C++ programming.
		
	Acknowledgement:
	The author would like to thank and acknowledge the advice, critical
	review, insight, and assistance provided by David Nash in the development
	of this work.		

********************************************************************************

	Implementation of the stdafx headers precompiled for this application

********************************************************************************

	This file is used for precompiled hearders. Precompiled headers allow
	programs to be recompiled more quickly

	To use precompiled headers, do the following:
	1) Add the set of rarely changed headers to stdafx.h
	2) Include stdafx.h in each cpp file. It must be included first.
	3) Add stdafx.cpp to your project.
	4) Turn on precompiled hearders in the project

*=============================================================================*/

#include "stdafx.h"
