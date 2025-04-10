#include "win_reg_util.h"

#include <Windows.h>

bool reg_key_exists(HKEY hkey, const std::wstring& subkey)
{
    return bool();
}

bool reg_value_exists(HKEY hkey, const std::wstring& subkey, const std::wstring& name)
{
    HKEY hk_sub;
    LSTATUS lret;
    if (ERROR_SUCCESS == (lret = RegOpenKeyEx(hkey, subkey.data(), 0, KEY_READ, &hk_sub))) {
        DWORD valType;
        DWORD valSize;
        if (ERROR_SUCCESS == (lret = RegQueryValueEx(hk_sub, name.data(), 0, &valType, 0, &valSize))) {
            RegCloseKey(hk_sub);
            return true;
        }
        RegCloseKey(hk_sub);
    }
    return false;
}
void reg_del_value(HKEY hkey, const std::wstring& subkey, const std::wstring& name)
{
    HKEY hk_sub;
    LSTATUS lret;
    if (ERROR_SUCCESS == (lret = RegOpenKeyEx(hkey, subkey.data(), 0, KEY_WRITE, &hk_sub))) {
        if (ERROR_SUCCESS == (lret = RegDeleteValue(hk_sub, name.data()))) {
            RegCloseKey(hk_sub);
        }
        RegCloseKey(hk_sub);
    }
    if (ERROR_SUCCESS != lret) {
        throw std::exception("Failed to delete the reg value");
    }
}
std::wstring reg_get_str(HKEY hkey, const std::wstring& subkey, const std::wstring& name)
{
    HKEY hk_sub;
    LSTATUS lret;
    std::wstring result;
    if (ERROR_SUCCESS == (lret = RegOpenKeyEx(hkey, subkey.data(), 0, KEY_READ, &hk_sub))) {
        DWORD valType;
        DWORD valSize;
        if (ERROR_SUCCESS == (lret = RegQueryValueEx(hk_sub, name.data(), 0, &valType, 0, &valSize))) {
            wchar_t* szbuf = new wchar_t[valSize + 1] { 0 };
            if (ERROR_SUCCESS == (lret = RegQueryValueEx(hk_sub, name.data(), 0, &valType, (LPBYTE)szbuf, &valSize))) {
                result.assign(szbuf);
            }
            delete[] szbuf;
        }
        RegCloseKey(hk_sub);
    }
    return result;
}
void reg_set_value(HKEY hkey, const std::wstring& subkey, const std::wstring& name, void* pVal, size_t lenVal, DWORD valType)
{
}
void reg_set_value(HKEY hkey, const std::wstring& subkey, const std::wstring& name, void* pVal, DWORD lenVal, DWORD valType = REG_SZ)
{
    LSTATUS lret;

    if (ERROR_SUCCESS == (lret = RegSetKeyValue(hkey, subkey.data(), name.data(), valType, pVal, lenVal))) {
    } else {
        throw std::exception("Failed to set the reg value");
    }
}
void reg_set_str(HKEY hkey, const std::wstring& subkey, const std::wstring& name, const std::wstring& val, DWORD valType)
{
    reg_set_value(hkey, subkey, name, (void*)val.data(), (DWORD)(val.length() * sizeof(typename std::wstring::value_type)));
}
