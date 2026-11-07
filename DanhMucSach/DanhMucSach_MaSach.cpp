#include "../DanhMucSach.h"
#include <cstring>
#include <stdexcept>

// ============================================================================
// PHÂN HỆ: QUẢN LÝ DANH MỤC SÁCH (DSLK ĐƠN)
// FILE: DanhMucSach_MaSach.cpp — Sinh mã sách tự động [ISBN]_[STT]
// PHỤ TRÁCH: LÊ ANH ĐỨC (N24DECE012)
// ============================================================================

// Sinh mã sách tự động kết hợp từ ISBN và số thứ tự
std::string sinhMaSach(const char *ISBN, int stt) {
    if (!ISBN || !*ISBN || std::strlen(ISBN) >= sizeof(DauSach::ISBN) ||
        std::strpbrk(ISBN, "_ \t\r\n") || stt < 1)
        throw std::invalid_argument("ISBN hoac so thu tu khong hop le");
    std::string ma = std::string(ISBN) + "_" + std::to_string(stt);
    if (ma.size() >= sizeof(DanhMucSach::maSach))
        throw std::length_error("Ma sach vuot suc chua cua CauTruc.h");
    return ma;
}
