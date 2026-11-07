#include "../DauSach.h"
#include "DauSach_NoiBo.h"
#include "../Common/Chuoi.h"
#include <cstring>

// ============================================================================
// PHÂN HỆ: QUẢN LÝ ĐẦU SÁCH (MẢNG CON TRỎ)
// FILE: DauSach_Them.cpp — Chức năng Thêm đầu sách (giữ thứ tự theo tên)
// PHỤ TRÁCH: LÊ ANH ĐỨC (N24DECE012)
// ============================================================================

// Chèn đầu sách mới vào mảng con trỏ có thứ tự tăng dần theo tên sách
bool themDauSach(DanhSachDauSach &ds, DauSach *moi) {
    using DauSachNoiBo::chuoiHopLe;
    if (!moi || ds.n < 0 || dauSachDay(ds)) return false;
    if (!chuoiHopLe(moi->ISBN) || !chuoiHopLe(moi->tenSach) ||
        !chuoiHopLe(moi->tacGia) || !chuoiHopLe(moi->theLoai) ||
        moi->soTrang < 1 || moi->namXuatBan < 1000 || moi->namXuatBan > 2100) return false;
    if (!Chuoi::hopLe(moi->ISBN) || !Chuoi::hopLe(moi->tenSach) ||
        !Chuoi::hopLe(moi->tacGia) || !Chuoi::hopLe(moi->theLoai)) return false;
    if (!Chuoi::coNoiDung(moi->tenSach) || !Chuoi::coNoiDung(moi->tacGia) ||
        !Chuoi::coNoiDung(moi->theLoai)) return false;
    if (std::strpbrk(moi->ISBN, "_ \t\r\n") ||
        timDauSachTheoISBN(ds, moi->ISBN)) return false;

    int viTri = 0;
    while (viTri < ds.n && soSanhTen(ds.nodes[viTri]->tenSach, moi->tenSach) <= 0)
        ++viTri;
    for (int i = ds.n; i > viTri; --i) ds.nodes[i] = ds.nodes[i - 1];
    ds.nodes[viTri] = moi;
    ++ds.n;
    voHieuChiMucISBN();
    return true;
}
