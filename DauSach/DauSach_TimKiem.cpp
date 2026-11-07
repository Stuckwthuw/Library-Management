#include "../DauSach.h"
#include "../DanhMucSach.h"
#include "DauSach_NoiBo.h"
#include <cstring>
#include <stdexcept>
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

// ============================================================================
// PHÂN HỆ: QUẢN LÝ ĐẦU SÁCH (MẢNG CON TRỎ)
// FILE: DauSach_TimKiem.cpp — Tìm kiếm đầu sách theo ISBN, tên sách và mã sách
// PHỤ TRÁCH: LÊ ANH ĐỨC (N24DECE012)
// ============================================================================

namespace {
const DanhSachDauSach *chuChiMuc = nullptr;
int soPhanTu = -1;
DauSach *chiMuc[MAX_DAUSACH];

// Xây dựng chỉ mục mảng phụ theo ISBN để tìm kiếm nhị phân
void taoChiMuc(const DanhSachDauSach &ds) {
    if (chuChiMuc == &ds && soPhanTu == ds.n) return;
    for (int i = 0; i < ds.n; ++i) chiMuc[i] = ds.nodes[i];
    DauSachNoiBo::sapXep(chiMuc, 0, ds.n - 1, [](DauSach *a, DauSach *b) {
        return std::strcmp(a->ISBN, b->ISBN) < 0;
    });
    chuChiMuc = &ds;
    soPhanTu = ds.n;
}

std::wstring chuThuong(const std::string &s) {
#ifdef _WIN32
    if (s.empty()) return L"";
    int n = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
                              s.c_str(), static_cast<int>(s.size()), nullptr, 0);
    if (n <= 0) throw std::invalid_argument("Chuoi khong phai UTF-8 hop le");
    std::wstring ketQua(n, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, s.c_str(), static_cast<int>(s.size()), &ketQua[0], n);
    CharLowerBuffW(&ketQua[0], n);
    return ketQua;
#else
    std::wstring ketQua(s.begin(), s.end());
    for (std::size_t i = 0; i < ketQua.size(); ++i)
        if (ketQua[i] >= L'A' && ketQua[i] <= L'Z') ketQua[i] += 32;
    return ketQua;
#endif
}
}

void voHieuChiMucISBN() { chuChiMuc = nullptr; soPhanTu = -1; }

// So sánh tên tiếng Việt không phân biệt hoa thường
int soSanhTen(const char *a, const char *b) {
#ifdef _WIN32
    std::wstring x = chuThuong(a), y = chuThuong(b);
    int k = CompareStringEx(L"vi-VN", 0, x.c_str(), -1, y.c_str(), -1,
                            nullptr, nullptr, 0);
    if (k) return k - CSTR_EQUAL;
#endif
    return std::strcmp(a, b);
}

// Kiểm tra tên sách có chứa từ khóa tìm kiếm hay không
bool tenChuaTuKhoa(const char *ten, const std::string &tuKhoa) {
    return ten && !tuKhoa.empty() &&
           chuThuong(ten).find(chuThuong(tuKhoa)) != std::wstring::npos;
}

// Tách ISBN từ mã sách ([ISBN]_[STT])
std::string trichXuatISBN(const char *maSach) {
    if (!maSach) return "";
    std::string s = maSach;
    return s.substr(0, s.find('_'));
}

// Tìm kiếm nhị phân đầu sách theo ISBN
DauSach *timDauSachTheoISBN(const DanhSachDauSach &ds, const char *ISBN) {
    if (!ISBN || !*ISBN) return nullptr;
    taoChiMuc(ds);
    int trai = 0, phai = ds.n - 1;
    while (trai <= phai) {
        int giua = trai + (phai - trai) / 2;
        int cmp = std::strcmp(chiMuc[giua]->ISBN, ISBN);
        if (cmp == 0) return chiMuc[giua];
        if (cmp < 0) trai = giua + 1;
        else phai = giua - 1;
    }
    return nullptr;
}

// Tìm đầu sách và bản sao sách theo mã sách cụ thể
bool timSachTheoMaSach(const DanhSachDauSach &ds, const char *maSach,
                      DauSach *&dauSach, NodeDMS *&cuonSach) {
    dauSach = timDauSachTheoISBN(ds, trichXuatISBN(maSach).c_str());
    cuonSach = dauSach ? timKiemDMS(dauSach->dms, maSach) : nullptr;
    if (!cuonSach) { dauSach = nullptr; return false; }
    return true;
}

// Tìm kiếm các đầu sách có tên chứa từ khóa
int timDauSachTheoTen(const DanhSachDauSach &ds, const char *tuKhoa,
                     DauSach **ketQua, int sucChua) {
    if (sucChua < ds.n || (ds.n && !ketQua) || ketQua == ds.nodes) return -1;
    if (!tuKhoa || !*tuKhoa) return 0;
    int n = 0;
    for (int i = 0; i < ds.n; ++i)
        if (tenChuaTuKhoa(ds.nodes[i]->tenSach, tuKhoa)) ketQua[n++] = ds.nodes[i];
    return n;
}
