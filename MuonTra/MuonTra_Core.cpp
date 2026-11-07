#include "../MuonTra.h"
#include "../Date.h"
#include <cstring>
#include <iostream>

using namespace std;

// ============================================================================
// PHÂN HỆ: GIAO DỊCH MƯỢN TRẢ (DSLK ĐƠN)
// FILE: MuonTra_Core.cpp — Các hàm phụ trợ mượn trả & kết nối module Date
// PHỤ TRÁCH: LÊ VĂN HOÀNG HIẾU (N24DECE018)
// ============================================================================

// Lấy ngày hiện tại của hệ thống
Date LayNgayHienTai() {
  return ngayHienTaiHeThong();
}

// Tính khoảng cách giữa 2 ngày (d2 - d1)
int TinhKhoangCachNgay(Date d1, Date d2) {
  return khoangCachNgay(d1, d2);
}

// Đếm số sách đang mượn của một độc giả
int DemSoSachDangMuon(NodeMuonTra *dsMuonTra) {
  int count = 0;
  NodeMuonTra *pCurr = dsMuonTra;
  while (pCurr != nullptr) {
    if (pCurr->data.trangThai == 0)
      count++;
    pCurr = pCurr->pNext;
  }
  return count;
}

// Kiểm tra độc giả có sách mượn quá hạn (> 7 ngày) hay không
bool KiemTraCoSachQuaHan(NodeMuonTra *dsMuonTra) {
  Date homNay = LayNgayHienTai();
  NodeMuonTra *pCurr = dsMuonTra;
  while (pCurr != nullptr) {
    if (pCurr->data.trangThai == 0) {
      if (!ngayHopLe(pCurr->data.ngayMuon)) return true;
      int soNgayMuon = TinhKhoangCachNgay(pCurr->data.ngayMuon, homNay);
      if (soNgayMuon > MAX_NGAY_MUON)
        return true;
    }
    pCurr = pCurr->pNext;
  }
  return false;
}

// Kiểm tra 3 điều kiện mượn sách: thẻ mở, mượn < 3 cuốn, không có sách quá hạn
KetQuaKiemTraMuon KiemTraDieuKienMuon(NodeDocGia *pDG) {
  if (!pDG) return LOI_DU_LIEU_MUON;
  if (pDG->data.trangThai != 1)
    return LOI_THE_BI_KHOA;

  Date homNay = LayNgayHienTai();
  int soSachDangMuon = 0;
  NodeMuonTra *pCurr = pDG->data.dsMuonTra;

  while (pCurr != nullptr) {
    if (pCurr->data.trangThai == 0) {
      if (!ngayHopLe(pCurr->data.ngayMuon) ||
          khoangCachNgay(pCurr->data.ngayMuon, homNay) < 0) return LOI_DU_LIEU_MUON;
      soSachDangMuon++;

      int soNgayMuon = TinhKhoangCachNgay(pCurr->data.ngayMuon, homNay);
      if (soNgayMuon > MAX_NGAY_MUON)
        return LOI_CO_SACH_QUA_HAN;
    }
    pCurr = pCurr->pNext;
  }

  if (soSachDangMuon >= MAX_MUON_SACH)
    return LOI_DA_MUON_TOI_DA;

  return DU_DIEU_KIEN_MUON;
}

// Tách mã ISBN từ mã sách ([ISBN]_[STT])
void TachISBN(const char *maSach, char *isbn) {
  if (maSach == nullptr || isbn == nullptr)
    return;

  const char *underscore = strchr(maSach, '_');
  if (underscore == nullptr) {
    strcpy(isbn, maSach);
    return;
  }

  int len = underscore - maSach;
  strncpy(isbn, maSach, len);
  isbn[len] = '\0';
}
