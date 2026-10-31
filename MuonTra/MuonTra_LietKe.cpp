#include "../DocGia.h"
#include "../MuonTra.h"
#include <cstdio>
#include <cstring>
#include <iostream>

using namespace std;

// ============================================================================
// PHÂN HỆ: GIAO DỊCH MƯỢN TRẢ
// FILE: MuonTra_LietKe.cpp — Chức năng (h): Liệt kê sách đang mượn (Ghép nối DMS)
// PHỤ TRÁCH: LÊ VĂN HOÀNG HIẾU (N24DECE018)
// ============================================================================

// [HÀM LIỆT KÊ SÁCH ĐANG MƯỢN — PHIÊN BẢN GHÉP NỐI ĐẦY ĐỦ DMS]
// Tra cứu tên sách thực tế từ mảng con trỏ DauSach của Đức để in bảng đẹp mắt.
void LietKeSachDangMuon(TREE_DocGia root, const DanhSachDauSach &dsDS,
                        int maThe) {
  // Bước 1: Tìm độc giả trên cây BST
  NodeDocGia *pDG = TimKiemDocGia(root, maThe);
  if (pDG == nullptr) {
    cout << "[LOI] Khong tim thay doc gia co ma the: " << maThe << "\n";
    return;
  }

  // In thông tin độc giả
  printf("\n===== DANH SACH SACH DANG MUON =====\n");
  printf("Doc gia: %s %s (Ma the: %d)\n", pDG->data.ho, pDG->data.ten, maThe);

  // Bước 2: Duyệt DSLK lọc các giao dịch đang mượn
  NodeMuonTra *pCurr = pDG->data.dsMuonTra;
  int stt = 0;

  // In header bảng
  printf(
      "+-----+---------------+------------------------------+------------+\n");
  printf(
      "| STT |   Ma Sach     |         Ten Sach              | Ngay Muon  |\n");
  printf(
      "+-----+---------------+------------------------------+------------+\n");

  while (pCurr != nullptr) {
    if (pCurr->data.trangThai == 0) { // Đang mượn
      stt++;

      // Tra cứu tên sách bên module Đức
      DauSach *pDauSach = nullptr;
      NodeDMS *pCuonSach = nullptr;
      const char *tenSach = "(Khong ro ten sach)";
      if (timSachTheoMaSach(dsDS, pCurr->data.maSach, pDauSach, pCuonSach) &&
          pDauSach != nullptr) {
        tenSach = pDauSach->tenSach;
      }

      // In dòng dữ liệu đã căn chỉnh
      printf("| %-3d | %-13s | %-28.28s | %02d/%02d/%04d |\n", stt,
             pCurr->data.maSach, tenSach, pCurr->data.ngayMuon.ngay,
             pCurr->data.ngayMuon.thang, pCurr->data.ngayMuon.nam);
    }
    pCurr = pCurr->pNext;
  }

  printf(
      "+-----+---------------+------------------------------+------------+\n");

  if (stt == 0) {
    printf("| Doc gia hien khong muon cuon sach nao.                          "
           "|\n");
    printf("+-----+---------------+------------------------------+------------+"
           "\n");
  } else {
    printf("Tong cong: %d cuon dang muon.\n", stt);
  }
}

// [HÀM LIỆT KÊ SÁCH ĐANG MƯỢN — PHIÊN BẢN ĐỘC LẬP / UNIT TEST]
void LietKeSachDangMuon(TREE_DocGia root, int maThe) {
  NodeDocGia *pDG = TimKiemDocGia(root, maThe);
  if (pDG == nullptr) {
    cout << "[LOI] Khong tim thay doc gia co ma the: " << maThe << "\n";
    return;
  }

  printf("\n===== DANH SACH SACH DANG MUON =====\n");
  printf("Doc gia: %s %s (Ma the: %d)\n", pDG->data.ho, pDG->data.ten, maThe);

  NodeMuonTra *pCurr = pDG->data.dsMuonTra;
  int stt = 0;

  printf(
      "+-----+---------------+------------------------------+------------+\n");
  printf(
      "| STT |   Ma Sach     |         Ten Sach              | Ngay Muon  |\n");
  printf(
      "+-----+---------------+------------------------------+------------+\n");

  while (pCurr != nullptr) {
    if (pCurr->data.trangThai == 0) {
      stt++;
      printf("| %-3d | %-13s | %-28s | %02d/%02d/%04d |\n", stt,
             pCurr->data.maSach, "(Chua nap DauSach)", pCurr->data.ngayMuon.ngay,
             pCurr->data.ngayMuon.thang, pCurr->data.ngayMuon.nam);
    }
    pCurr = pCurr->pNext;
  }

  printf(
      "+-----+---------------+------------------------------+------------+\n");
  if (stt == 0) {
    printf("| Doc gia hien khong muon cuon sach nao.                          |\n");
    printf("+-----+---------------+------------------------------+------------+\n");
  } else {
    printf("Tong cong: %d cuon dang muon.\n", stt);
  }
}
