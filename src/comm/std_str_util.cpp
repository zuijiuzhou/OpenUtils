#include "std_str_util.h"

#include <regex>
#include <stdarg.h>

#define __TSTR_IS_STRING__ constexpr(std::is_same<TStr, std::string>::value)
#define __TSTR_IS_WSTRING__ constexpr(std::is_same<TStr, std::wstring>::value)
#define __TSTR_TYPE_ERROR__ static_assert("only support std::string or std::wstring")

std::wstring str_to_wstr(const std::string& str)
{
    size_t charLen = str.length() + 1;
    size_t memSize = charLen * sizeof(wchar_t);
    wchar_t* szbuf = (wchar_t*)malloc(memSize);
    memset(szbuf, 0, memSize);
    size_t n = 0;
    errno_t err = mbstowcs_s(&n, szbuf, charLen, str.data(), _TRUNCATE);
    std::wstring result(szbuf);
    free(szbuf);
    return result;
}
std::string wstr_to_str(const std::wstring& wstr)
{
    size_t charLen = wstr.length() + 1;
    size_t memSize = charLen * sizeof(char);
    char* szbuf = (char*)malloc(memSize);
    memset(szbuf, 0, memSize);
    size_t n = 0;
    errno_t err = wcstombs_s(&n, szbuf, charLen, wstr.data(), _TRUNCATE);
    std::string result(szbuf);
    free(szbuf);
    return result;
}

__TSTR_TMPL__ TStr str_to_tstr(const std::string& str)
{
    if __TSTR_IS_STRING__ {
        return str;
    } else if __TSTR_IS_WSTRING__ {
        return str_to_wstr(str);
    } else {
        __TSTR_TYPE_ERROR__;
        return TStr();
    }
}
__TSTR_TMPL__ TStr wstr_to_tstr(const std::wstring& wstr)
{
    if __TSTR_IS_STRING__ {
        return wstr_to_str(wstr);
    } else if __TSTR_IS_WSTRING__ {
        return wstr;
    } else {
        __TSTR_TYPE_ERROR__;
        return TStr();
    }
}
__TSTR_TMPL__ std::string tstr_to_str(const TStr& tstr)
{
    if __TSTR_IS_STRING__ {
        return tstr;
    } else if __TSTR_IS_WSTRING__ {
        return wstr_to_str(tstr);
    } else {
        __TSTR_TYPE_ERROR__;
        return std::string();
    }
}
__TSTR_TMPL__ std::wstring tstr_to_wstr(const TStr& tstr)
{
    if __TSTR_IS_STRING__ {
        return str_to_wstr(tstr);
    } else if __TSTR_IS_WSTRING__ {
        return tstr;
    } else {
        return std::wstring();
        // __TSTR_TYPE_ERROR__;
    }
}
__TSTR_TMPL__ int tstr_cmp_icase(const TStr& left, const TStr& right)
{
    size_t min_num_chars = left.length() < right.length() ? left.length() : right.length();
    if (min_num_chars == 0) {
        return left.length() > right.length() ? 1 : -1;
    }
    for (size_t i = 0; i < min_num_chars; i++) {
        int n = towupper(left.at(i)) - towupper(right.at(i));
        if (n == 0) {
            continue;
        }
        return n > 0 ? 1 : -1;
    }
    return left.length() == right.length() ? 0 : (left.length() > right.length() ? 1 : -1);
}
__TSTR_TMPL__ void tstr_replace(TStr& str, const TStr& from, const TStr& to)
{
    if (str.empty() || from.empty()) {
        return;
    }
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != TStr::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}
__TSTR_TMPL__ TStr tstr_join(std::vector<TStr> items, TStr seperator)
{
    bool isfirst = true;
    TStr result;
    for (auto& item : items) {
        if (isfirst) {
            isfirst = false;
        } else {
            result += seperator;
        }
        result += item;
    }
    return result;
}
__TSTR_TMPL__ TStr tstr_trim_end(const TStr& tstr, typename TStr::value_type theChar)
{
    auto chrIdx = (int)tstr.length() - 1;
    while (chrIdx >= 0) {
        if (tstr.at(chrIdx) == theChar) {
            chrIdx--;
        } else {
            break;
        }
    }
    if (chrIdx > 0) {
        return TStr(tstr, 0, chrIdx + 1);
    }
    return TStr();
}
__TSTR_TMPL__ TStr tstr_trim_start(const TStr& tstr, typename TStr::value_type theChar)
{
    TStr ret = tstr;
    auto foundAt = std::find_if(ret.begin(), ret.end(), [theChar](typename TStr::value_type c) { return theChar != c; });

    ret.erase(ret.begin(), foundAt);
    return ret;
}
__TSTR_TMPL__ TStr tstr_trim(const TStr& tstr, typename TStr::value_type theChar)
{
    return tstr_trim_start(tstr_trim_end(tstr, theChar), theChar);
}
__TSTR_TMPL__ void tstr_split(const TStr& data, std::vector<TStr>& ret)
{
    tstr_split(data, 32, ret);
}
__TSTR_TMPL__ void tstr_split(const TStr& data, typename TStr::value_type seperator, std::vector<TStr>& ret)
{
    TStr sep(1, seperator);
    std::basic_regex<typename TStr::value_type> reg(sep);
    std::regex_token_iterator<typename TStr::const_iterator> iter(data.begin(), data.end(), reg, (int)_TRUNCATE);
    ret.assign(iter, std::regex_token_iterator<typename TStr::const_iterator>());
    // std::vector<TStr> items = std::vector<TStr>(iter, std::regex_token_iterator<typename TStr::const_iterator>());
}
__TSTR_TMPL__ TStr tstr_format(const typename TStr::value_type* fmt, ...)
{
    TStr ret;
    va_list args;
    va_start(args, fmt);
    int n = 0;
    if __TSTR_IS_STRING__ {
        n = vsnprintf(0, 0, fmt, args);
    } else if __TSTR_IS_WSTRING__ {
        n = vswprintf(0, 0, fmt, args);
    }
    va_end(args);

    va_start(args, fmt);
    if (n > ret.capacity()) {
        ret.reserve(n + 1);
    }
    if __TSTR_IS_STRING__ {
        n = vsnprintf(ret.data(), ret.capacity(), fmt, args);
    } else if __TSTR_IS_WSTRING__ {
        n = vswprintf(ret.data(), ret.capacity(), fmt, args);
    }
    va_end(args);
    return TStr(ret.data());
}
__TSTR_TMPL__ bool tstr_is_clear(const TStr& tstr)
{
    static const std::basic_regex<typename TStr::value_type> reg(str_to_tstr<TStr>("^\\s*$"));
    return tstr.length() == 0 || std::regex_match(tstr, reg);
};

template std::string str_to_tstr<std::string>(const std::string& str);
template std::wstring str_to_tstr<std::wstring>(const std::string& str);
template std::string wstr_to_tstr<std::string>(const std::wstring& wstr);
template std::wstring wstr_to_tstr<std::wstring>(const std::wstring& wstr);
template std::string tstr_to_str<std::string>(const std::string& tstr);
template std::string tstr_to_str<std::wstring>(const std::wstring& tstr);
template std::wstring tstr_to_wstr<std::string>(const std::string& tstr);
template std::wstring tstr_to_wstr<std::wstring>(const std::wstring& tstr);
template int tstr_cmp_icase<std::string>(const std::string& left, const std::string& right);
template int tstr_cmp_icase<std::wstring>(const std::wstring& left, const std::wstring& right);
template void tstr_replace<std::string>(std::string& str, const std::string& from, const std::string& to);
template void tstr_replace<std::wstring>(std::wstring& str, const std::wstring& from, const std::wstring& to);
template std::string tstr_join<std::string>(std::vector<std::string> items, std::string seperator);
template std::wstring tstr_join<std::wstring>(std::vector<std::wstring> items, std::wstring seperator);
template std::string tstr_trim_end<std::string>(const std::string& tstr, char theChar);
template std::wstring tstr_trim_end<std::wstring>(const std::wstring& tstr, wchar_t theChar);
template std::string tstr_trim_start<std::string>(const std::string& tstr, char theChar);
template std::wstring tstr_trim_start<std::wstring>(const std::wstring& tstr, wchar_t theChar);
template std::string tstr_trim<std::string>(const std::string& tstr, char theChar);
template std::wstring tstr_trim<std::wstring>(const std::wstring& tstr, wchar_t theChar);
template void tstr_split<std::string>(const std::string& data, std::vector<std::string>& ret);
template void tstr_split<std::wstring>(const std::wstring& data, std::vector<std::wstring>& ret);
template void tstr_split<std::string>(const std::string& data, char seperator, std::vector<std::string>& ret);
template void tstr_split<std::wstring>(const std::wstring& data, wchar_t seperator, std::vector<std::wstring>& ret);
template std::string tstr_format<std::string>(const char* fmt, ...);
template std::wstring tstr_format<std::wstring>(const wchar_t* fmt, ...);
template bool tstr_is_clear<std::string>(const std::string& tstr);
template bool tstr_is_clear<std::wstring>(const std::wstring& tstr);

#undef __TSTR_IS_STRING__
#undef __TSTR_IS_WSTRING__
#undef __TSTR_TYPE_ERROR__