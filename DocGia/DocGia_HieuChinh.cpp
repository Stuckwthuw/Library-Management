#include "../DocGia.h"
#include <cstring>
#include <iostream>

using namespace std;

// ============================================================================
// PHÂN HỆ: QUẢN LÝ THẺ ĐỘC GIẢ (CÂY NHỊ PHÂN TÌM KIẾM - BST)
// FILE: DocGia_HieuChinh.cpp - Chức năng Hiệu chỉnh thông tin độc giả
// PHỤ TRÁCH: LÊ VĂN HOÀNG HIẾU (N24DECE018)
// ============================================================================

// [HÀM HIỆU CHỈNH THÔNG TIN ĐỘC GIẢ]
// Cập nhật thông tin họ tên, phái và trạng thái thẻ của độc giả (bảo toàn maThe).
// Tối ưu nhập liệu: Tự động định dạng Title Case cho họ và tên mới,
// bẫy lỗi trạng thái hợp lệ (chỉ nhận 0 hoặc 1).
bool HieuChinhDocGia(TREE_DocGia root, int maThe, const char *ho,
                     const char *ten, const char *phai, int trangThai) {
  NodeDocGia *pDG = TimKiemDocGia(root, maThe);
  if (pDG == nullptr) {
    cout << "[LOI] Khong tim thay doc gia co ma the: " << maThe << "\n";
    return false;
  }

  // Ràng buộc giá trị trạng thái hợp lệ
  if (trangThai != 0 && trangThai != 1) {
    cout << "[LOI] Trang thai the chi chap nhan 1 (Hoat dong) hoac 0 (Khoa)!\n";
    return false;
  }

  // Bẫy lỗi họ tên mới: không được chứa chữ số hoặc ký tự đặc biệt
  if (!KiemTraChuCaiVaKhoangTrang(ho)) {
    cout << "[LOI] Ho moi khong hop le (khong duoc chua chu so hoac ky tu dac biet)!\n";
    return false;
  }
  if (!KiemTraChuCaiVaKhoangTrang(ten)) {
    cout << "[LOI] Ten moi khong hop le (khong duoc chua chu so hoac ky tu dac biet)!\n";
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

  // Tự động chuẩn hóa định dạng chữ hoa đầu từ cho họ và tên
  DinhDangHoTen(pDG->data.ho);
  DinhDangHoTen(pDG->data.ten);

  return true;
}
