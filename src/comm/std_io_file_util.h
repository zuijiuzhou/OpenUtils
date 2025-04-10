#pragma once

#include "comm_global.h"

#include <cstdio>
#include <string>
#include <vector>

#include "text.h"

COMM_EXPORT TextEncoding file_get_encoding(const char* buf, size_t lenBuf);
COMM_EXPORT std::vector<std::wstring> file_read_lines(FILE* pFile);