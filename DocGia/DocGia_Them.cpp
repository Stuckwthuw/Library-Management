#include "../DocGia.h"
#include <iostream>
#include <cstring>
#include <new>

using namespace std;

// ============================================================================
// PHÂN HỆ: QUẢN LÝ THẺ ĐỘC GIẢ (CÂY NHỊ PHÂN TÌM KIẾM - BST)
// FILE: DocGia_Them.cpp - Chức năng Thêm thẻ độc giả mới
// PHỤ TRÁCH: LÊ VĂN HOÀNG HIẾU (N24DECE018)
// ============================================================================

// Thêm thẻ độc giả mới vào cây BST (đệ quy)
bool ThemDocGia(TREE_DocGia &root, TheDocGia dg) {
  if (!std::memchr(dg.ho, '\0', sizeof(dg.ho)) ||
      !std::memchr(dg.ten, '\0', sizeof(dg.ten)) ||
      !std::memchr(dg.phai, '\0', sizeof(dg.phai)) ||
      dg.maThe < 1 || dg.maThe > 999999 ||
      (dg.trangThai != 0 && dg.trangThai != 1) ||
      (strcmp(dg.phai, "Nam") != 0 && strcmp(dg.phai, "Nu") != 0) ||
      !DinhDangHoTen(dg.ho) || !DinhDangHoTen(dg.ten)) return false;

  if (!KiemTraChuCaiVaKhoangTrang(dg.ho)) {
    cout << "[LOI] Ho va ten dem khong hop le (khong duoc chua chu so hoac ky tu dac biet)!\n";
    return false;
  }
  if (!KiemTraChuCaiVaKhoangTrang(dg.ten)) {
    cout << "[LOI] Ten doc gia khong hop le (khong duoc chua chu so hoac ky tu dac biet)!\n";
    return false;
  }

  if (root == nullptr) {
    NodeDocGia *pNew = new (std::nothrow) NodeDocGia;
    if (pNew == nullptr) {
      cout << "[LOI] Khong du bo nho de cap phat node moi!\n";
      return false;
    }
    pNew->data = dg;
    pNew->data.dsMuonTra = dg.dsMuonTra;
    pNew->pLeft = nullptr;
    pNew->pRight = nullptr;
    root = pNew;
    return true;
  }

  if (dg.maThe == root->data.maThe) {
    cout << "[LOI] Ma the " << dg.maThe << " da ton tai trong he thong!\n";
    return false;
  }

  if (dg.maThe < root->data.maThe)
    return ThemDocGia(root->pLeft, dg);
  return ThemDocGia(root->pRight, dg);
}
