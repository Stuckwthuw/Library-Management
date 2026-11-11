#include "../DocGia.h"
#include "../MuonTra.h"
#include "../DauSach.h"
#include "../DanhMucSach.h"
#include "../Date.h"
#include <cstring>
#include <iostream>

// ============================================================================
// PHÂN HỆ: GIAO DỊCH MƯỢN TRẢ
// FILE: MuonTra_Tra.cpp — Chức năng Trả sách & Báo mất sách
// PHỤ TRÁCH: LÊ VĂN HOÀNG HIẾU (N24DECE018)
// ============================================================================

namespace {
// Cập nhật trạng thái kết thúc lượt mượn (trả sách hoặc báo mất)
bool ketThucMuon(TREE_DocGia root, DanhSachDauSach &ds, int maThe,
                 const char *maSach, bool mat) {
    if (!KiemTraDinhDangMaSach(maSach)) {

        return false;
    }
    NodeDocGia *dg = TimKiemDocGia(root, maThe);
    if (!dg) {  return false; }
    NodeMuonTra *giaoDich = dg->data.dsMuonTra;
    while (giaoDich && (giaoDich->data.trangThai != 0 ||
           std::strcmp(giaoDich->data.maSach, maSach))) giaoDich = giaoDich->pNext;
    DauSach *sach = nullptr;
    NodeDMS *cuon = nullptr;
    if (!giaoDich || !timSachTheoMaSach(ds, maSach, sach, cuon) || !cuon || cuon->data.trangThai != 1) {

        return false;
    }
    Date homNay = LayNgayHienTai();
    if (!ngayHopLe(giaoDich->data.ngayMuon) || khoangCachNgay(giaoDich->data.ngayMuon, homNay) < 0) {

        return false;
    }
    giaoDich->data.ngayTra = homNay;
    giaoDich->data.trangThai = mat ? 2 : 1;
    capNhatTrangThaiSach(cuon, mat ? 2 : 0);

    return true;
}
}

// Xử lý trả sách
bool TraSach(TREE_DocGia root, DanhSachDauSach &ds, int maThe, const char *maSach) {
    return ketThucMuon(root, ds, maThe, maSach, false);
}

// Xử lý báo mất sách
bool BaoMatSach(TREE_DocGia root, DanhSachDauSach &ds, int maThe, const char *maSach) {
    return ketThucMuon(root, ds, maThe, maSach, true);
}
