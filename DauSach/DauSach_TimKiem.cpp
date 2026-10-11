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

namespace {
const DanhSachDauSach *chuChiMuc = nullptr;
int soPhanTu = -1;
DauSach *chiMuc[MAX_DAUSACH];

// Mảng chính bắt buộc sắp theo tên: dùng mảng phụ để tìm nhị phân theo ISBN.
// Xây O(N log N) sau lần thay đổi, các truy vấn tiếp theo O(log N).
void taoChiMuc(const DanhSachDauSach &ds) {
    if (chuChiMuc == &ds && soPhanTu == ds.n) return;
    for (int i = 0; i < ds.n; ++i) chiMuc[i] = ds.nodes[i];
    DauSachNoiBo::sapXep(chiMuc, 0, ds.n - 1, [](DauSach *a, DauSach *b) {
        return std::strcmp(a->ISBN, b->ISBN) < 0;
    });
    chuChiMuc = &ds;
    soPhanTu = ds.n;
}

// Đổi UTF-8 sang UTF-16 để Windows xử lý hoa/thường tiếng Việt đúng từng ký tự.
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

// So sánh tên theo tiếng Việt trên Windows; giữ nguyên dấu của tên lưu trữ.
int soSanhTen(const char *a, const char *b) {
#ifdef _WIN32
    std::wstring x = chuThuong(a), y = chuThuong(b);
    int k = CompareStringEx(L"vi-VN", 0, x.c_str(), -1, y.c_str(), -1,
                            nullptr, nullptr, 0);
    if (k) return k - CSTR_EQUAL;
#endif
    return std::strcmp(a, b);
}

bool tenChuaTuKhoa(const char *ten, const std::string &tuKhoa) {
    return ten && !tuKhoa.empty() &&
           chuThuong(ten).find(chuThuong(tuKhoa)) != std::wstring::npos;
}

std::string trichXuatISBN(const char *maSach) {
    if (!maSach) return "";
    std::string s = maSach;
    return s.substr(0, s.find('_'));
}

// Trả nullptr khi danh sách rỗng hoặc không tìm được ISBN.
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

// O(log N + M) sau khi có chỉ mục; M là số cuốn của đầu sách cần tìm.
// Không duyệt tất cả DMS vì tiền tố mã xác định duy nhất đầu sách.
bool timSachTheoMaSach(const DanhSachDauSach &ds, const char *maSach,
                      DauSach *&dauSach, NodeDMS *&cuonSach) {
    dauSach = timDauSachTheoISBN(ds, trichXuatISBN(maSach).c_str());
    cuonSach = dauSach ? timKiemDMS(dauSach->dms, maSach) : nullptr;
    if (!cuonSach) { dauSach = nullptr; return false; }
    return true;
}

// Duyệt O(N) vì tìm chuỗi con không có tính đơn điệu để tìm nhị phân.
// Kết quả giữ thứ tự tên và cho phép truy cập đầy đủ dms để in mục e.
int timDauSachTheoTen(const DanhSachDauSach &ds, const char *tuKhoa,
                     DauSach **ketQua, int sucChua) {
    if (sucChua < ds.n || (ds.n && !ketQua) || ketQua == ds.nodes) return -1;
    if (!tuKhoa || !*tuKhoa) return 0;
    int n = 0;
    for (int i = 0; i < ds.n; ++i)
        if (tenChuaTuKhoa(ds.nodes[i]->tenSach, tuKhoa)) ketQua[n++] = ds.nodes[i];
    return n;
}
