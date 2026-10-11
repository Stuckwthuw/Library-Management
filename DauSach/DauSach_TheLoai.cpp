#include "../DauSach.h"
#include "DauSach_NoiBo.h"
#include <cstring>

// Sắp mảng con trỏ tạm theo (thể loại, tên), O(N log N) trung bình.
// Không sắp lại mảng chính, nhờ vậy luôn giữ ràng buộc tăng dần theo tên sách.
int layDauSachTheoTheLoai(const DanhSachDauSach &ds, DauSach **ketQua, int sucChua) {
    if (sucChua < ds.n || (ds.n && !ketQua) || ketQua == ds.nodes) return -1;
    for (int i = 0; i < ds.n; ++i) ketQua[i] = ds.nodes[i];
    DauSachNoiBo::sapXep(ketQua, 0, ds.n - 1, [](DauSach *a, DauSach *b) {
        int cmp = soSanhTen(a->theLoai, b->theLoai);
        if (cmp) return cmp < 0;
        // Giữ các thể loại có cách viết khác nhau thành từng nhóm nhất quán.
        cmp = std::strcmp(a->theLoai, b->theLoai);
        if (cmp) return cmp < 0;
        cmp = soSanhTen(a->tenSach, b->tenSach);
        return cmp ? cmp < 0 : std::strcmp(a->ISBN, b->ISBN) < 0;
    });
    return ds.n;
}
