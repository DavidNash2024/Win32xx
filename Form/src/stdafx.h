#ifndef STDAFX_H
#define STDAFX_H

/** @file    StdAfx.h
 *  @brief   Include file for standard system include files,
 *           or project specific include files that are used frequently, but
 *           are changed infrequently.
 *           Used as the 'landmark' for pre-compiled headers.
 *  @author  Lynn Allan
 *  @version 2.32
 *  @date    2009-May-08
 */
#pragma once

#define WIN32_LEAN_AND_MEAN    // Exclude rarely-used stuff from Windows headers

// This avoid warning from vc7.1 about targeting for IE 5
// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#if _MSC_VER > 1200
#define WINVER 0x0400
#endif

#define VC_EXTRALEAN    // Exclude rarely-used stuff from Windows headers

typedef  unsigned char  byte;

#pragma warning (disable : 4786)
#pragma warning (disable : 4702)
#include <vector>
#pragma warning (default : 4702)
#include <map>
#pragma warning (default : 4786)
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <assert.h>

#include <windows.h>
#include <commctrl.h>
#include <tchar.h>

#include <dialog.h>
#include <docking.h>
#include <frame.h>
#include <gdi.h>
#include <listview.h>
#include <mdi.h>
#include <propertysheet.h>
#include <rebar.h>
#include <socket.h>
#include <splitter.h>
#include <statusbar.h>
#include <toolbar.h>
#include <treeview.h>
#include <wincore.h>

#endif
