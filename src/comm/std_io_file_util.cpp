#include "std_io_file_util.h"

#include <codecvt>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

TextEncoding file_get_encoding(const char* buf, size_t lenBuf)
{
    if (lenBuf < 2) {
        return TE_UNKNOW;
    }
    TextEncoding te = TE_UNKNOW;

    unsigned char c0 = *buf,
                  c1 = *(buf + 1),
                  c2 = *(buf + 2);
    if (lenBuf >= 3 && c0 == 0xef && c1 == 0xbb && c2 == 0xbf) {
        te = TE_UTF8;
    } else if (lenBuf >= 2 && c0 == 0xff && c1 == 0xfe) {
        te = TE_UTF16_LE;
    } else if (lenBuf >= 2 && c0 == 0xfe && c1 == 0xff) {
        te = TE_UTF16_BE;
    } else {
        bool isutf8 = true;
        const char* start = buf;
        const char* end = start + lenBuf;
        while (start < end) {
            if (*start < 0x80) {
                start++;
            } else if (*start < 0xc0) {
                isutf8 = false;
                break;
            } else if (*start < 0xe0) {
                if (start >= end - 1) {
                    break;
                }
                if ((start[1] & 0xc0) != 0x80) {
                    isutf8 = false;
                    break;
                }
                start += 2;
            } else if (*start < 0xf0) {
                if (start >= end - 2) {
                    break;
                }
                if ((start[1] & 0xc0) != 0x80 || (start[2] & 0xc0) != 0x80) {
                    isutf8 = false;
                    break;
                }
                start += 3;
            } else {
                isutf8 = false;
                break;
            }
        }
        if (isutf8) {
            te = TE_UTF8;
        } else {
            te = TE_GBK;
        }
    }
    return te;
}

std::vector<std::wstring> file_read_lines(FILE* pFile)
{
    fseek(pFile, 0, SEEK_END);
    long lenFile = ftell(pFile);
    fseek(pFile, 0, SEEK_SET);
    std::shared_ptr<char> buf(new char[lenFile + 1] { 0 }, [](char* p) { delete[] p; });

    size_t lenRead = fread(buf.get(), sizeof(char), lenFile, pFile);
    TextEncoding te = file_get_encoding(buf.get(), lenFile);

    std::wstring str;
    std::wstring line;
    if (te == TE_GBK) {
        std::wstring_convert<std::codecvt_byname<wchar_t, char, std::mbstate_t>> cvt_gbk_u16(new std::codecvt_byname<wchar_t, char, mbstate_t>(".936"));
        str = cvt_gbk_u16.from_bytes(buf.get());
    } else if (te == TE_UTF8) {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> cvt_utf8_utf16;
        str = cvt_utf8_utf16.from_bytes(buf.get());
    } else if (te == TE_UTF16_LE) {
        wchar_t* wbuf = new wchar_t[lenFile / 2 + 1] { 0 };
        memcpy(wbuf, buf.get(), lenFile);
        str.assign(wbuf);
        delete[] wbuf;
    } else if (te == TE_UTF16_BE) {
        wchar_t* wbuf = new wchar_t[lenFile / 2 + 1] { 0 };
        memcpy(wbuf, buf.get(), lenFile);
        for (size_t i = 0; i < lenFile / 2; i++) {
            wbuf[i] = _byteswap_ushort(wbuf[i]);
        }
        str.assign(wbuf);
        delete[] wbuf;
    }
    std::vector<std::wstring> lines;
    std::wistringstream iss(str);
    while (std::getline(iss, line)) {
        lines.push_back(line);
    }
    return lines;
}