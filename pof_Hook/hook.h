#ifndef ___HOOK_H_INCLUDED__
#define ___HOOK_H_INCLUDED__

#ifdef _MSC_VER
#pragma once
#endif 

#define METHOD(message) toPrintFile(__FUNCTION__, __FILE__, __LINE__, (message));

#include <windows.h>
#include "data.h"
#include "hook_target.h"
#include <windows.h>
#include <iostream>
#include "../Logger/Logger.h"
//#include "data.h"
//#include "hook_target.h"
//#include "Disassembler\hde32.h"
//#include <windows.h>
//#include <stdio.h>
//#include <intrin.h>

class Hook
{

private:
	
	#define COUNT 3

	//https://www.malwaretech.com/2015/01/inline-hooking-for-programmers-part-1.html
	const int f_size = 20;
	const int jump_size = 5;

	Logger* logger = NULL;
	LPVOID mem;
	DATA data[COUNT];
	int dataL = sizeof(data) / sizeof(DATA);
	BOOL _disable_(CHAR *_lib_, CHAR *_function_, LPVOID _target_, LPVOID _source_, PDWORD _len_);
	BOOL _enable_(CHAR *_lib_, CHAR *_function_, LPVOID _target_, LPVOID _source_, PDWORD _len_);
	

public:
	Hook();
	void Enable();
	void Disable();
	~Hook();
};

#endif



