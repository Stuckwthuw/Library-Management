#include "../DanhMucSach.h"
#include <cstring>
#include <stdexcept>

// Tiền tố ISBN xác định đầu sách, STT phân biệt từng cuốn.
// '_' dành để phân tách; chặn tràn maSach[20], không cắt mã.
std::string sinhMaSach(const char *ISBN, int stt) {
    if (!ISBN || !*ISBN || std::strlen(ISBN) >= sizeof(DauSach::ISBN) ||
        std::strpbrk(ISBN, "_ \t\r\n") || stt < 1)
        throw std::invalid_argument("ISBN hoac so thu tu khong hop le");
    std::string ma = std::string(ISBN) + "_" + std::to_string(stt);
    if (ma.size() >= sizeof(DanhMucSach::maSach))
        throw std::length_error("Ma sach vuot suc chua cua CauTruc.h");
    return ma;
}
