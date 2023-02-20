#pragma once

#include <string>
#include <vector>
#include <functional>

#ifdef _MSVC_LANG
#if (_MSVC_LANG >= 202002L)
#define __CXX_STD_20__
#endif
#endif

#define __API__
#define __TSTR_IS_STRING__ constexpr(std::is_same<TStr, std::string>::value)
#define __TSTR_IS_WSTRING__ constexpr(std::is_same<TStr, std::wstring>::value)

#ifdef __CXX_STD_20__
#define __TSTR_FUNC__ template <typename TStr> \
    requires std::is_same<TStr, std::string>::value || std::is_same<TStr, std::wstring>::value
#else
#define __TSTR_FUNC__ template <typename TStr>
#endif

#define __TSTR_TYPE_ERROR__ static_assert("only support std::string or std::wstring")

enum TextEncoding
{
    TE_UNKNOW = 0,
    TE_ASC2,
    TE_UTF8,
    TE_UTF16_LE,
    TE_UTF16_BE,
    TE_GB2312 = 1000,
    TE_GBK
};

std::wstring str_to_wstr(const std::string &str);
std::string wstr_to_str(const std::wstring &wstr);
__TSTR_FUNC__ TStr str_to_tstr(const std::string &str);
__TSTR_FUNC__ TStr wstr_to_tstr(const std::wstring &wstr);
__TSTR_FUNC__ std::string tstr_to_str(const TStr &tstr);
__TSTR_FUNC__ std::wstring tstr_to_wstr(const TStr &tstr);
__TSTR_FUNC__ int tstr_cmp_icase(const TStr &left, const TStr &right);
__TSTR_FUNC__ void tstr_replace(TStr &str, const TStr &from, const TStr &to);
__TSTR_FUNC__ TStr tstr_join(std::vector<TStr> items, TStr seperator);
__TSTR_FUNC__ TStr tstr_trim_end(const TStr &tstr, typename TStr::value_type theChar = 32);
__TSTR_FUNC__ TStr tstr_trim_start(const TStr &tstr, typename TStr::value_type theChar = 32);
__TSTR_FUNC__ TStr tstr_trim(const TStr &tstr, typename TStr::value_type theChar = 32);

std::wstring str_to_wstr(const std::string &str)
{
    size_t charLen = str.length() + 1;
    size_t memSize = charLen * sizeof(wchar_t);
    wchar_t *szbuf = (wchar_t *)malloc(memSize);
    memset(szbuf, 0, memSize);
    size_t n = 0;
    errno_t err = mbstowcs_s(&n, szbuf, charLen, str.data(), _TRUNCATE);
    std::wstring result(szbuf);
    free(szbuf);
    return result;
}
std::string wstr_to_str(const std::wstring &wstr)
{
    size_t charLen = wstr.length() + 1;
    size_t memSize = charLen * sizeof(char);
    char *szbuf = (char *)malloc(memSize);
    memset(szbuf, 0, memSize);
    size_t n = 0;
    errno_t err = wcstombs_s(&n, szbuf, charLen, wstr.data(), _TRUNCATE);
    std::string result(szbuf);
    free(szbuf);
    return result;
}
__TSTR_FUNC__ TStr str_to_tstr(const std::string &str)
{
    if __TSTR_IS_STRING__
    {
        return str;
    }
    else if __TSTR_IS_WSTRING__
    {
        return str_to_wstr(str);
    }
    else
    {
        __TSTR_TYPE_ERROR__;
        return TStr();
    }
}
__TSTR_FUNC__ TStr wstr_to_tstr(const std::wstring &wstr)
{
    if __TSTR_IS_STRING__
    {
        return wstr_to_str(wstr);
    }
    else if __TSTR_IS_WSTRING__
    {
        return wstr;
    }
    else
    {
        __TSTR_TYPE_ERROR__;
        return TStr();
    }
}
__TSTR_FUNC__ std::string tstr_to_str(const TStr &tstr)
{
    if __TSTR_IS_STRING__
    {
        return tstr;
    }
    else if __TSTR_IS_WSTRING__
    {
        return wstr_to_str(tstr);
    }
    else
    {
        __TSTR_TYPE_ERROR__;
        return std::string();
    }
}
__TSTR_FUNC__ std::wstring tstr_to_wstr(const TStr &tstr)
{
    if __TSTR_IS_STRING__
    {
        return str_to_wstr(tstr);
    }
    else if __TSTR_IS_WSTRING__
    {
        return tstr;
    }
    else
    {
        return std::wstring();
        // __TSTR_TYPE_ERROR__;
    }
}
__TSTR_FUNC__ int tstr_cmp_icase(const TStr &left, const TStr &right)
{
    size_t min_num_chars = min(left.length(), right.length());
    if (min_num_chars == 0)
    {
        return left.length() > right.length() ? 1 : -1;
    }
    for (size_t i = 0; i < min_num_chars; i++)
    {
        int n = towupper(left.at(i)) - towupper(right.at(i));
        if (n == 0)
        {
            continue;
        }
        return n > 0 ? 1 : -1;
    }
    return left.length() == right.length() ? 0 : (left.length() > right.length() ? 1 : -1);
}
__TSTR_FUNC__ void tstr_replace(TStr &str, const TStr &from, const TStr &to)
{
    if (from.empty())
    {
        return;
    }
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != TStr::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}
__TSTR_FUNC__ TStr tstr_join(std::vector<TStr> items, TStr seperator)
{
    bool isfirst = true;
    TStr result;
    for (auto &item : items)
    {
        if (isfirst)
        {
            isfirst = false;
        }
        else
        {
            result += seperator;
        }
        result += item;
    }
    return result;
}
__TSTR_FUNC__ TStr tstr_trim_end(const TStr &tstr, typename TStr::value_type theChar)
{
    auto chrIdx = tstr.length() - 1;
    while (chrIdx >= 0)
    {
        if (tstr.at(chrIdx) == theChar)
        {
            chrIdx--;
        }
        else
        {
            break;
        }
    }
    if (chrIdx > 0)
    {
        return TStr(tstr, 0, chrIdx + 1);
    }
    return TStr();
}
__TSTR_FUNC__ TStr tstr_trim_start(const TStr &tstr, typename TStr::value_type theChar)
{
    TStr ret = tstr;
    ret.erase(ret.begin(), std::find_if(tstr.begin(), tstr.end(), [theChar](typename TStr::value_type c)
                                        { return theChar != c; }));
    return ret;
}
__TSTR_FUNC__ TStr tstr_trim(const TStr &tstr, typename TStr::value_type theChar)
{
    return tstr_trim_start(tstr_trim_end(tstr, theChar), theChar);
}

#define __TSTR__(STDSTR) str_to_tstr<TStr>(STDSTR)