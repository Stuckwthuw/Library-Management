#include "../DocGia.h"
#include <cstring>
#include <iostream>

using namespace std;

// ============================================================================
// PHÂN HỆ: QUẢN LÝ THẺ ĐỘC GIẢ (CÂY NHỊ PHÂN TÌM KIẾM - BST)
// FILE: DocGia_HieuChinh.cpp - Chức năng Hiệu chỉnh thông tin độc giả
// ============================================================================

// [HÀM HIỆU CHỈNH THÔNG TIN ĐỘC GIẢ]
// Dùng strncpy và đặt ký tự kết thúc chuỗi '\0' để chống tràn bộ đệm.
bool HieuChinhDocGia(TREE_DocGia root, int maThe, const char *ho,
                     const char *ten, const char *phai, int trangThai) {
  NodeDocGia *pDG = TimKiemDocGia(root, maThe);
  if (pDG == nullptr) {
    cout << "[LOI] Khong tim thay doc gia co ma the: " << maThe << "\n";
    return false;
  }

  // Cập nhật các trường được phép thay đổi (bảo toàn maThe)
  strncpy(pDG->data.ho, ho, sizeof(pDG->data.ho) - 1);
  strncpy(pDG->data.ten, ten, sizeof(pDG->data.ten) - 1);
  strncpy(pDG->data.phai, phai, sizeof(pDG->data.phai) - 1);
  pDG->data.trangThai = trangThai;

  // Đảm bảo chuỗi kết thúc bằng ký tự null (tránh rác bộ nhớ khi in)
  pDG->data.ho[sizeof(pDG->data.ho) - 1] = '\0';
  pDG->data.ten[sizeof(pDG->data.ten) - 1] = '\0';
  pDG->data.phai[sizeof(pDG->data.phai) - 1] = '\0';

  return true;
}
