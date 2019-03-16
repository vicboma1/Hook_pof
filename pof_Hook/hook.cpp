#include "hook.h"
#include "Disassembler\hde32.h"
#include <iostream>
#include <intrin.h>
#include <windows.h>
#include <StrSafe.h>
#include <Tchar.h.>
#include <string>

using namespace std;

#define NO_INLINE_ASM
#define _TRAZA_LOG_ (std::string(__FUNCTION__) + std::string("  L: ") + std::string("__LINE__") + std::string(" | ") + std::string(__FILE__) + std::string("\n")).c_str() 

TdefOldMessageBox OldMessageBox;
TdefOldMessageBoxW OldMessageBoxW;
TdefOldMessageBoxA OldMessageBoxA;

/*
[...]
*/

Hook::Hook()
{
	(this->logger) = new Logger("Hook.log", "w");
	logger->toPrintFileln(_TRAZA_LOG_);

	data[0] = { "user32.dll", "MessageBox",  (LPVOID)&NewMessageBox,  &OldMessageBox,  0 };
	data[1] = { "user32.dll", "MessageBoxW", (LPVOID)&NewMessageBoxW, &OldMessageBoxW, 0 };
	data[2] = { "user32.dll", "MessageBoxA", (LPVOID)&NewMessageBoxA, &OldMessageBoxA, 0 };
}

void Hook::Enable()
{
	logger->toPrintFileln(_TRAZA_LOG_);

	mem = VirtualAlloc(NULL, (jump_size + f_size) * dataL, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (!mem)
		return;

	for (auto i = 0; i < dataL; i++)
	{
		*(LPVOID *)data[i]._SOURCE_ = (LPVOID)((DWORD)mem + (i * (jump_size + f_size)));
		_enable_(data[i]._LIB_, data[i]._FUNCTION_, data[i]._TARGET_, *(LPVOID *)data[i]._SOURCE_, &data[i]._LEN_);
	}
}

BOOL Hook::_enable_(CHAR *_lib_, CHAR *_function_, LPVOID _target_, LPVOID _source_, PDWORD _len_)
{
	logger->toPrintFileln(_TRAZA_LOG_);

	DWORD flNewProtect = 0;
	LPVOID FAddress;
	DWORD lineL = 0;
	hde32s disam;
	BYTE Jump[5] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };

	FAddress = GetProcAddress(GetModuleHandleA(_lib_), _function_);
	if (!FAddress)
		return FALSE;

	memcpy(_source_, FAddress, lineL);
	*(DWORD *)(Jump + 1) = ((DWORD)FAddress + lineL) - ((DWORD)_source_ + lineL + jump_size);
	memcpy((LPVOID)((DWORD)_source_ + lineL), Jump, jump_size);

	if (!VirtualProtect(FAddress, lineL, PAGE_EXECUTE_READWRITE, &flNewProtect))
		return FALSE;

	*(DWORD *)(Jump + 1) = (DWORD)_target_ - (DWORD)FAddress - jump_size;
	
	BYTE SourceBuffer[8];
	if (jump_size > 8)
		return;

	memcpy(SourceBuffer, FAddress, 8);
	memcpy(SourceBuffer, Jump, jump_size);

	VirtualProtect(FAddress, lineL, flNewProtect, &flNewProtect);

	FlushInstructionCache(GetCurrentProcess(), FAddress, lineL);

	*_len_ = lineL;
	return TRUE;
}

void Hook::Disable()
{
	logger->toPrintFileln(_TRAZA_LOG_);

	for (auto i = 0; i < dataL; i++)
		_disable_(data[i]._LIB_, data[i]._FUNCTION_, *(LPVOID *)data[i]._SOURCE_, &data[i]._LEN_);

	VirtualFree(mem, 0, MEM_RELEASE);
}

BOOL Hook::_disable_(CHAR *_lib_, CHAR *_function_, LPVOID _source_, DWORD _len_)
{
	logger->toPrintFileln(_TRAZA_LOG_);

	LPVOID FAddress;
	DWORD flNewProtect;

	FAddress = GetProcAddress(GetModuleHandleA(_lib_), _function_);
	if (!FAddress)
		return FALSE;

	if (!VirtualProtect(FAddress, _len_, PAGE_EXECUTE_READWRITE, &flNewProtect))
		return FALSE;

	BYTE SourceBuffer[8];
	if (_len_ > 8)
		return;

	memcpy(SourceBuffer, FAddress, 8);
	memcpy(SourceBuffer, _source_, _len_);

	VirtualProtect(FAddress, _len_, PAGE_EXECUTE_READWRITE, &flNewProtect);

	FlushInstructionCache(GetCurrentProcess(), FAddress, _len_);

	return TRUE;
}

Hook::~Hook()
{
	logger->toPrintFileln(_TRAZA_LOG_);

	if (this->logger)
		logger->~Logger();

	if (this)
		free(this);
}
