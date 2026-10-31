#include "../DocGia.h"
#include <iostream>

using namespace std;

// ============================================================================
// PHÂN HỆ: QUẢN LÝ THẺ ĐỘC GIẢ (CÂY NHỊ PHÂN TÌM KIẾM - BST)
// FILE: DocGia_Them.cpp - Chức năng Thêm thẻ độc giả mới vào cây BST
// PHỤ TRÁCH: LÊ VĂN HOÀNG HIẾU (N24DECE018)
// ============================================================================

// [HÀM THÊM ĐỘC GIẢ MỚI VÀO CÂY BST - ĐỆ QUY]
// Giải thuật: Đệ quy so sánh maThe với khóa node hiện tại để tìm vị trí lá
// trống phù hợp rồi cấp phát động và cắm node mới vào đó.
// Tối ưu nhập liệu: Tự động chuẩn hóa Họ và Tên (Title Case, trim space)
// trước khi lưu vào node cây.
bool ThemDocGia(TREE_DocGia &root, TheDocGia dg) {
  // Chuẩn hóa định dạng Họ và Tên độc giả trước khi đưa vào cây
  DinhDangHoTen(dg.ho);
  DinhDangHoTen(dg.ten);

  // Bẫy lỗi nghiệp vụ: Họ tên không được rỗng, không chứa chữ số hoặc ký tự đặc biệt
  if (!KiemTraChuCaiVaKhoangTrang(dg.ho)) {
    cout << "[LOI] Ho va ten dem khong hop le (khong duoc chua chu so hoac ky tu dac biet)!\n";
    return false;
  }
  if (!KiemTraChuCaiVaKhoangTrang(dg.ten)) {
    cout << "[LOI] Ten doc gia khong hop le (khong duoc chua chu so hoac ky tu dac biet)!\n";
    return false;
  }

  if (root == nullptr) {
    // Tìm được vị trí lá trống phù hợp, tiến hành cấp phát node mới
    NodeDocGia *pNew = new NodeDocGia;
    if (pNew == nullptr) {
      // Bẫy lỗi: Hết bộ nhớ RAM
      cout << "[LOI] Khong du bo nho de cap phat node moi!\n";
      return false;
    }
    pNew->data = dg;
    pNew->data.dsMuonTra =
        dg.dsMuonTra; // Giữ nguyên DSLK đã nạp (khi đọc file)
    pNew->pLeft = nullptr;
    pNew->pRight = nullptr;
    root = pNew;
    return true;
  }

  // Kiểm tra trùng khóa - không cho thêm mã thẻ đã tồn tại
  if (dg.maThe == root->data.maThe) {
    cout << "[LOI] Ma the " << dg.maThe << " da ton tai trong he thong!\n";
    return false;
  }

  // Đệ quy tìm vị trí: nhỏ hơn đi sang nhánh trái, lớn hơn đi sang nhánh phải
  if (dg.maThe < root->data.maThe)
    return ThemDocGia(root->pLeft, dg);
  return ThemDocGia(root->pRight, dg);
}
