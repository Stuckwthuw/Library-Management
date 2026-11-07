#include "../DauSach.h"
#include "DauSach_NoiBo.h"
#include <cstring>
#include <memory>

// ============================================================================
// PHÂN HỆ: QUẢN LÝ ĐẦU SÁCH (MẢNG CON TRỎ)
// FILE: DauSach_Top10.cpp — Thống kê Top 10 đầu sách có lượt mượn nhiều nhất
// PHỤ TRÁCH: LÊ ANH ĐỨC (N24DECE012)
// ============================================================================

namespace {
// Đếm số lượt mượn từ danh sách liên kết mượn trả
void demDanhSach(const NodeMuonTra *head, ThongKeSach *a, int n) {
    for (const NodeMuonTra *p = head; p; p = p->pNext) {
        std::string ISBN = trichXuatISBN(p->data.maSach);
        int trai = 0, phai = n - 1;
        while (trai <= phai) {
            int giua = trai + (phai - trai) / 2;
            int cmp = std::strcmp(a[giua].sach->ISBN, ISBN.c_str());
            if (cmp == 0) { ++a[giua].soLuotMuon; break; }
            if (cmp < 0) trai = giua + 1;
            else phai = giua - 1;
        }
    }
}

// Duyệt cây BST độc giả để đếm lượt mượn
void demLichSu(const NodeDocGia *root, ThongKeSach *a, int n) {
    if (!root) return;
    demLichSu(root->pLeft, a, n);
    demDanhSach(root->data.dsMuonTra, a, n);
    demLichSu(root->pRight, a, n);
}
}

// Thống kê số lượt mượn của tất cả các đầu sách
bool thongKeLuotMuon(const DanhSachDauSach &ds, const NodeDocGia *root,
                    ThongKeSach *ketQua, int sucChua, const NodeMuonTra *luuTru) {
    if (sucChua < ds.n || (ds.n && !ketQua)) return false;
    for (int i = 0; i < ds.n; ++i) ketQua[i] = ThongKeSach{ds.nodes[i], 0};
    DauSachNoiBo::sapXep(ketQua, 0, ds.n - 1, [](const ThongKeSach &a, const ThongKeSach &b) {
        return std::strcmp(a.sach->ISBN, b.sach->ISBN) < 0;
    });
    demLichSu(root, ketQua, ds.n);
    demDanhSach(luuTru, ketQua, ds.n);
    DauSachNoiBo::sapXep(ketQua, 0, ds.n - 1, [](const ThongKeSach &a, const ThongKeSach &b) {
        if (a.soLuotMuon != b.soLuotMuon) return a.soLuotMuon > b.soLuotMuon;
        int cmp = soSanhTen(a.sach->tenSach, b.sach->tenSach);
        return cmp ? cmp < 0 : std::strcmp(a.sach->ISBN, b.sach->ISBN) < 0;
    });
    return true;
}

// Lấy danh sách Top 10 đầu sách mượn nhiều nhất
int layTop10(const DanhSachDauSach &ds, const NodeDocGia *root,
             ThongKeSach *ketQua, int sucChua, const NodeMuonTra *luuTru) {
    int n = ds.n < 10 ? ds.n : 10;
    if (sucChua < n || (n && !ketQua)) return -1;
    if (n == 0) return 0;
    std::unique_ptr<ThongKeSach[]> tam(new ThongKeSach[ds.n]);
    thongKeLuotMuon(ds, root, tam.get(), ds.n, luuTru);
    for (int i = 0; i < n; ++i) ketQua[i] = tam[i];
    return n;
}
