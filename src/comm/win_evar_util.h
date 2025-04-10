#pragma once

#ifdef _WIN32

#include "comm_global.h"

#include <string>
#include <unordered_map>

#include <Windows.h>

COMM_EXPORT std::wstring evar_get(const std::wstring& name, bool global = false);
COMM_EXPORT bool         evar_exists(const std::wstring& name, bool global = false);
COMM_EXPORT void evar_set(const std::wstring& name, const std::wstring& val, DWORD valType = REG_SZ, bool global = false);
COMM_EXPORT void evar_del(const std::wstring& name, bool global = false);
COMM_EXPORT std::unordered_map<std::wstring, std::wstring> evar_list(bool global = false);

#endif // _WIN32