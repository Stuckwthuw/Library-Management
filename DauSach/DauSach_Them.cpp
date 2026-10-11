#include "../DauSach.h"
#include "DauSach_NoiBo.h"
#include <cstring>

// Chèn O(N), dịch địa chỉ thay vì sao chép DauSach có con trỏ sở hữu DMS.
// Kiểm tra đầy, null, trường rỗng và ISBN trùng trước khi thay đổi danh sách.
bool themDauSach(DanhSachDauSach &ds, DauSach *moi) {
    using DauSachNoiBo::chuoiHopLe;
    if (!moi || ds.n < 0 || dauSachDay(ds)) return false;
    if (!chuoiHopLe(moi->ISBN) || !chuoiHopLe(moi->tenSach) ||
        !chuoiHopLe(moi->tacGia) || !chuoiHopLe(moi->theLoai) ||
        moi->soTrang < 1 || moi->namXuatBan < 1) return false;
    if (std::strpbrk(moi->ISBN, "_ \t\r\n") ||
        timDauSachTheoISBN(ds, moi->ISBN)) return false;

    // Tìm vị trí trước rồi mới dịch mảng: lỗi so sánh/cấp phát không làm đứt dữ liệu.
    int viTri = 0;
    while (viTri < ds.n && soSanhTen(ds.nodes[viTri]->tenSach, moi->tenSach) <= 0)
        ++viTri;
    for (int i = ds.n; i > viTri; --i) ds.nodes[i] = ds.nodes[i - 1];
    ds.nodes[viTri] = moi;
    ++ds.n;
    voHieuChiMucISBN();
    return true;
}
