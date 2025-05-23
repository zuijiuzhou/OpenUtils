#pragma once

#include "comm_global.h"

#include <string>
#include <vector>

#define __TSTR_TMPL__ template <typename TStr> \
    requires std::is_same<TStr, std::string>::value || std::is_same<TStr, std::wstring>::value

COMM_EXPORT std::wstring str_to_wstr(const std::string& str);
COMM_EXPORT std::string wstr_to_str(const std::wstring& wstr);

__TSTR_TMPL__ TStr str_to_tstr(const std::string& str);
__TSTR_TMPL__ TStr wstr_to_tstr(const std::wstring& wstr);
__TSTR_TMPL__ std::string tstr_to_str(const TStr& tstr);
__TSTR_TMPL__ std::wstring tstr_to_wstr(const TStr& tstr);
__TSTR_TMPL__ int tstr_cmp_icase(const TStr& left, const TStr& right);
__TSTR_TMPL__ void tstr_replace(TStr& str, const TStr& from, const TStr& to);
__TSTR_TMPL__ TStr tstr_join(std::vector<TStr> items, TStr seperator);
__TSTR_TMPL__ TStr tstr_trim_end(const TStr& tstr, typename TStr::value_type theChar = 32);
__TSTR_TMPL__ TStr tstr_trim_start(const TStr& tstr, typename TStr::value_type theChar = 32);
__TSTR_TMPL__ TStr tstr_trim(const TStr& tstr, typename TStr::value_type theChar = 32);
__TSTR_TMPL__ void tstr_split(const TStr& data, std::vector<TStr>& ret);
__TSTR_TMPL__ void tstr_split(const TStr& data, typename TStr::value_type seperator, std::vector<TStr>& ret);
__TSTR_TMPL__ TStr tstr_format(const typename TStr::value_type* fmt, ...);
__TSTR_TMPL__ bool tstr_is_clear(const TStr& tstr);
