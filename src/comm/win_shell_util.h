#pragma once

#include "comm_global.h"

#include <string>

COMM_EXPORT int start_as_admin(const std::wstring& exe, const std::wstring& params);

COMM_EXPORT int restart_as_admin(bool waitForExit = true);
