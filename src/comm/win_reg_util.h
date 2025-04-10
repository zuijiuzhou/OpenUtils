#pragma once

#ifdef _WIN32

#include "comm_global.h"

#include <string>

#include <Windows.h>

COMM_EXPORT bool reg_key_exists(HKEY hkey, const std::wstring& subkey);
COMM_EXPORT bool reg_value_exists(HKEY hkey, const std::wstring& subkey, const std::wstring& name);
COMM_EXPORT void reg_del_value(HKEY hkey, const std::wstring& subkey, const std::wstring& name);
COMM_EXPORT std::wstring reg_get_str(HKEY hkey, const std::wstring& subkey, const std::wstring& name);
COMM_EXPORT void reg_set_value(HKEY hkey, const std::wstring& subkey, const std::wstring& name, void* pVal, size_t lenVal, DWORD valType = REG_SZ);
COMM_EXPORT void reg_set_str(HKEY hkey, const std::wstring& subkey, const std::wstring& name, const std::wstring& val, DWORD valType = REG_SZ);

#endif
