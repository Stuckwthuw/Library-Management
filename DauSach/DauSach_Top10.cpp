#include "../DauSach.h"
#include "DauSach_NoiBo.h"
#include <cstring>
#include <memory>

namespace {
// Chỉ đọc cây của Hiếu. Mỗi node mượn trả tương ứng một lượt, kể cả đã trả/mất.
// Tìm nhị phân ISBN trên mảng thống kê: O(L log N) cho L lượt mượn.
void demLichSu(const NodeDocGia *root, ThongKeSach *a, int n) {
    if (!root) return;
    demLichSu(root->pLeft, a, n);
    for (const NodeMuonTra *p = root->data.dsMuonTra; p; p = p->pNext) {
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
    demLichSu(root->pRight, a, n);
}
}

// Mảng phụ lưu số đếm, không sửa giao ước DauSach hoặc lịch sử của độc giả.
// Cây rỗng cho số lượt bằng 0; hòa lượt thì so tên, sau đó ISBN cho kết quả ổn định.
bool thongKeLuotMuon(const DanhSachDauSach &ds, const NodeDocGia *root,
                    ThongKeSach *ketQua, int sucChua) {
    if (sucChua < ds.n || (ds.n && !ketQua)) return false;
    for (int i = 0; i < ds.n; ++i) ketQua[i] = ThongKeSach{ds.nodes[i], 0};
    DauSachNoiBo::sapXep(ketQua, 0, ds.n - 1, [](const ThongKeSach &a, const ThongKeSach &b) {
        return std::strcmp(a.sach->ISBN, b.sach->ISBN) < 0;
    });
    demLichSu(root, ketQua, ds.n);
    DauSachNoiBo::sapXep(ketQua, 0, ds.n - 1, [](const ThongKeSach &a, const ThongKeSach &b) {
        if (a.soLuotMuon != b.soLuotMuon) return a.soLuotMuon > b.soLuotMuon;
        int cmp = soSanhTen(a.sach->tenSach, b.sach->tenSach);
        return cmp ? cmp < 0 : std::strcmp(a.sach->ISBN, b.sach->ISBN) < 0;
    });
    return true;
}

// Tối đa 10 đầu sách, hoặc ít hơn nếu thư viện chưa đủ 10 đầu sách.
// unique_ptr chỉ quản lý new[]/delete[], không thay thế bằng container STL.
int layTop10(const DanhSachDauSach &ds, const NodeDocGia *root,
             ThongKeSach *ketQua, int sucChua) {
    int n = ds.n < 10 ? ds.n : 10;
    if (sucChua < n || (n && !ketQua)) return -1;
    if (n == 0) return 0;
    std::unique_ptr<ThongKeSach[]> tam(new ThongKeSach[ds.n]);
    thongKeLuotMuon(ds, root, tam.get(), ds.n);
    for (int i = 0; i < n; ++i) ketQua[i] = tam[i];
    return n;
}
