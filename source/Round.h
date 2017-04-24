// Copyleft 2017 Akshay.C.S. Based on works by Chris Korda and others.
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 of the License, or any later version.

#pragma once

#ifdef _WIN64
#include "intrin.h"		// for SSE intrinsics
#endif

inline int round(double x)	// round to 32-bit integer
{
#ifdef _WIN64
    return(_mm_cvtsd_si32(_mm_set_sd(x)));
#else
	int		temp;
	__asm {
		fld		x		// load real
		fistp	temp	// store integer and pop stack
	}
	return(temp);
#endif
}

inline int trunc(double x)	// truncate to 32-bit integer
{
#ifdef _WIN64
    return(_mm_cvttsd_si32(_mm_set_sd(x)));
#else
	int		temp;
	short	cw, chop;
	__asm {
		fstcw	cw		// save control word
		mov		ax, cw
		or		ax, 0c00h	// set rounding mode to chop
		mov		chop, ax
		fldcw	chop	// load chop control word
		fld		x		// load real
		fistp	temp	// store integer and pop stack
		fldcw	cw		// restore control word
	}
	return(temp);
#endif
}

inline LONGLONG round64(double x)	// round to 64-bit integer
{
#ifdef _WIN64
    return(_mm_cvtsd_si64x(_mm_set_sd(x)));
#else
	LONGLONG	temp;
	__asm {
		fld		x		// load real
		fistp	temp	// store integer and pop stack
	}
	return(temp);
#endif
}

inline LONGLONG trunc64(double x)	// truncate to 64-bit integer
{
#ifdef _WIN64
    return(_mm_cvttsd_si64x(_mm_set_sd(x)));
#else
	LONGLONG	temp;
	short	cw, chop;
	__asm {
		fstcw	cw		// save control word
		mov		ax, cw
		or		ax, 0c00h	// set rounding mode to chop
		mov		chop, ax
		fldcw	chop	// load chop control word
		fld		x		// load real
		fistp	temp	// store integer and pop stack
		fldcw	cw		// restore control word
	}
	return(temp);
#endif
}

inline W64INT roundW64INT(double x)	// round to W64INT
{
#ifdef _WIN64
	return(round64(x));
#else
	return(round(x));
#endif
}

inline W64INT truncW64INT(double x)	// truncate to W64INT
{
#ifdef _WIN64
	return(trunc64(x));
#else
	return(trunc(x));
#endif
}
