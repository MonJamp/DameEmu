#pragma once

// Ignore warnings from wxWidgets
#pragma warning(push, 4)
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE 

#include <wx/wx.h>

#include <wx/wfstream.h>
#include <wx/aboutdlg.h>
#include <wx/listctrl.h>
#include <wx/itemattr.h>
#include <wx/notebook.h>
#include <wx/statline.h>

#if defined(_MSC_VER) && defined(DETECT_MEM_LEAK)
#include <wx/msw/msvcrt.h>
#endif

#undef _CRT_NONSTDC_NO_DEPRECATE 
#undef _CRT_SECURE_NO_DEPRECATE
#pragma warning(pop)