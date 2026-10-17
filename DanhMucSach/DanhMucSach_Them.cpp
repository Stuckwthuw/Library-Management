#include "../DanhMucSach.h"
#include <cstdlib>
#include <cstring>
#include <limits>
#include <cerrno>

// Lấy STT lớn nhất thay vì số node để tránh trùng khi dãy mã có khoảng trống.
// Giữ sách thanh lý trong DMS để không tái sử dụng mã.
// Tạo chuỗi tạm rồi nối một lần: lỗi giữa chừng không đổi danh sách cũ.
// O(M + K), với M cuốn cũ và K cuốn thêm.
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
        // Mã cuối dài nhất: kiểm tra trước khi cấp phát cả danh sách.
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
