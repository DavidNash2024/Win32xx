/////////////////////////////
// UserMessages.h
//

#pragma once

// Message - sent to the parent (Frame) window when a file is dropped on the View window
//   WPARAM: A pointer to the filename (LPCWSTR)
//   LPARAM: unused
#define UWM_DROPFILE (WM_APP + 0x0001)

// Message - sent to the parent (Frame) window to retrieve the PlotPoint data.
//   Returns a pointer to a vector of PlotPoint
#define UWM_GETALLPOINTS (WM_APP + 0x0002)

// Message - sent to the parent (Frame) window containing the PlotPoint information.
//   WPARAM: A pointer to PlotPoint.
//   LPARAM: unused
#define UWM_SENDPOINT (WM_APP + 0x0003)

