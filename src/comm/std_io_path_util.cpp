#include "std_io_path_util.h"

#include "std_str_util.h"

#define __PATH_SEPERATOR__ L';'

std::vector<std::wstring> path_split(const std::wstring& data) {
    // std::wcout << __FUNCTIONW__ << std::endl;
    std::vector<std::wstring> ret;
    tstr_split<std::wstring>(data, __PATH_SEPERATOR__, ret);
    return ret;
}
int path_compare(const std::wstring& left, const std::wstring& right) {
    std::wstring leftTemp  = path_norm(left);
    std::wstring rightTemp = path_norm(right);
    int          n         = tstr_cmp_icase<std::wstring>(leftTemp, rightTemp);
    return n;
}
std::wstring path_join(std::vector<std::wstring> items) {
    return tstr_join<std::wstring>(items, str_to_tstr<std::wstring>(std::string(1, __PATH_SEPERATOR__)));
}
std::wstring path_norm(const std::wstring& path) {
    std::wstring        ret = path;
    static std::wstring seperator_win(L"\\");
    static std::wstring seperator_unix(L"/");
    static std::wstring seperator_unix2(L"//");
    tstr_replace<std::wstring>(ret, seperator_win, seperator_unix);
    tstr_replace<std::wstring>(ret, seperator_unix2, seperator_unix);

    ret = tstr_trim(ret);
    ret = tstr_trim_end(ret, seperator_unix[0]);
    return ret;
}