//	Holds utility macros for SGD.
#pragma once

//	Macro to safely Release COM Objects.
#ifndef SAFE_RELEASE
	#define SAFE_RELEASE(p)			if (p) { p->Release(); p = NULL; }
#endif

//	Macro to safely delete pointers.
#ifndef SAFE_DELETE
	#define SAFE_DELETE(p)			if (p) { delete p; p = NULL; }
#endif

//	Macro to safely delete array pointers.
#ifndef SAFE_DELETE_ARRAY
	#define SAFE_DELETE_ARRAY(p)	if (p) { delete [] p; p = NULL; }
#endif

//	Macro for reporting Wrapper errors
#ifndef DXERROR
	#define DXERROR(a)	{ OutputDebugString(a); OutputDebugString("\n"); return false; }
#endif