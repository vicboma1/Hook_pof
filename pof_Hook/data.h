#ifndef ___DATA_H_INCLUDED__
#define ___DATA_H_INCLUDED__

#ifdef _MSC_VER
#pragma once
#endif 

#include <windows.h>

typedef struct {
	CHAR *_LIB_;
	CHAR *_FUNCTION_;
	LPVOID _TARGET_;
	LPVOID _SOURCE_;
	DWORD _LEN_;
} DATA;
#endif
