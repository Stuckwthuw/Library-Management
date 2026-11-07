#pragma once
#include <string>
#include <stdexcept>
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

// ============================================================================
// TIỆN ÍCH XỬ LÝ CHUỖI TIẾNG VIỆT UNICODE (WINDOWS CONSOLE)
// ============================================================================

namespace Chuoi {
inline std::wstring rong(const char *s) {
    if (!s) throw std::invalid_argument("Chuoi null");
    int n = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, s, -1, nullptr, 0);
    if (!n) throw std::invalid_argument("UTF-8 khong hop le");
    std::wstring result(n, L'\0');
    MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, s, -1, &result[0], n);
    result.resize(n - 1);
    return result;
}
inline std::string utf8(const std::wstring &s) {
    if (s.empty()) return "";
    int n = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, s.data(),
        static_cast<int>(s.size()), nullptr, 0, nullptr, nullptr);
    if (!n) throw std::invalid_argument("UTF-16 khong hop le");
    std::string result(n, '\0');
    WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, s.data(),
        static_cast<int>(s.size()), &result[0], n, nullptr, nullptr);
    return result;
}
inline bool hopLe(const char *s) {
    try { rong(s); return true; } catch (const std::invalid_argument &) { return false; }
}
inline bool coNoiDung(const char *s) {
    try {
        for (wchar_t c : rong(s)) {
            WORD type = 0;
            GetStringTypeW(CT_CTYPE1, &c, 1, &type);
            if (!(type & C1_SPACE)) return true;
        }
    } catch (const std::invalid_argument &) { return false; }
    return false;
}
inline std::wstring thuong(const char *s) {
    std::wstring w = rong(s);
    if (!w.empty()) CharLowerBuffW(&w[0], static_cast<DWORD>(w.size()));
    return w;
}
inline int soSanh(const char *a, const char *b) {
    std::wstring x = rong(a), y = rong(b);
    int result = CompareStringEx(L"vi-VN", NORM_IGNORECASE, x.c_str(), -1,
        y.c_str(), -1, nullptr, nullptr, 0);
    if (!result) throw std::runtime_error("Khong so sanh duoc chuoi");
    return result - CSTR_EQUAL;
}
}
