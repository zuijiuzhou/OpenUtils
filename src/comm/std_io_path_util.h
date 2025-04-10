#pragma once

#include "comm_global.h"

#include <string>
#include <vector>

COMM_EXPORT std::vector<std::wstring> path_split(const std::wstring& data);
COMM_EXPORT int path_compare(const std::wstring& left, const std::wstring& right);
COMM_EXPORT std::wstring path_join(std::vector<std::wstring> items);
COMM_EXPORT std::wstring path_norm(const std::wstring& path);
