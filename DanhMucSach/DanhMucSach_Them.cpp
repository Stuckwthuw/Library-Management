#include "../DanhMucSach.h"
#include <cstdlib>
#include <cstring>
#include <limits>
#include <cerrno>

// ============================================================================
// PHÂN HỆ: QUẢN LÝ DANH MỤC SÁCH (DSLK ĐƠN)
// FILE: DanhMucSach_Them.cpp — Chức năng Thêm các bản sao sách vào đầu sách
// PHỤ TRÁCH: LÊ ANH ĐỨC (N24DECE012)
// ============================================================================

// Thêm các bản sao sách mới vào danh mục sách của một đầu sách
bool themBanSao(DauSach &d, int n, const char *vt) {
    if (n < 1 || !vt || !*vt || !d.ISBN[0] ||
        !std::memchr(d.ISBN, '\0', sizeof(d.ISBN)) ||
        std::strlen(vt) >= sizeof(DanhMucSach::viTri))
        return false;
    long maxSTT = 0;
    NodeDMS *tail = nullptr;
    for (NodeDMS *p = d.dms; p; p = p->pNext) {
        tail = p;
        const std::string ma = p->data.maSach;
        const std::size_t pos = ma.rfind('_');
        if (pos == std::string::npos || ma.substr(0, pos) != d.ISBN) return false;
        char *end = nullptr;
        errno = 0;
        long stt = std::strtol(ma.c_str() + pos + 1, &end, 10);
        if (errno == ERANGE || !end || *end || stt < 1) return false;
        if (stt > maxSTT) maxSTT = stt;
    }
    if (maxSTT > std::numeric_limits<int>::max() - n) return false;
    NodeDMS *first = nullptr, *last = nullptr;
    try {
        sinhMaSach(d.ISBN, static_cast<int>(maxSTT) + n);
        for (int i = 0; i < n; ++i) {
            std::string ma = sinhMaSach(d.ISBN, static_cast<int>(maxSTT) + i + 1);
            NodeDMS *p = taoNodeDMS(ma.c_str(), 0, vt);
            if (last) last->pNext = p;
            else first = p;
            last = p;
        }
    } catch (...) {
        giaiPhongDMS(first);
        return false;
    }
    if (tail) tail->pNext = first;
    else d.dms = first;
    return true;
}
