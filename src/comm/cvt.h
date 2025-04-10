#pragma once

#include "comm_global.h"

#include "std_str_util.h"

template <typename TVal>
std::wstring cvt_to_wstr(TVal n, unsigned int maxFixed = -1, bool trimZero = false);

template <typename TVal>
std::wstring cvt_to_wstr(TVal n, unsigned int maxFixed, bool trimZero)
{
    if (maxFixed < 0)
    {
       return std::to_wstring(n);
    }

    std::wstring fmt = tstr_format<std::wstring>(L"%%.%df", maxFixed);
    std::wstring ret = tstr_format<std::wstring>(fmt.data(), n);

    if (trimZero)
    {
        ret = tstr_trim_end(tstr_trim_start(ret, L'0'), L'0');
    }
    return ret;
}