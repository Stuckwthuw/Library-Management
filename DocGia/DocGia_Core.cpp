#include "../DocGia.h"
#include "../Common/Chuoi.h"
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

// ============================================================================
// PHÂN HỆ: QUẢN LÝ THẺ ĐỘC GIẢ (CÂY NHỊ PHÂN TÌM KIẾM - BST)
// FILE: DocGia_Core.cpp - Các hàm cơ bản, tiện ích chuỗi và quản lý bộ nhớ
// PHỤ TRÁCH: LÊ VĂN HOÀNG HIẾU (N24DECE018)
// ============================================================================

// Kiểm tra chuỗi rỗng hoặc chỉ toàn khoảng trắng
bool KiemTraChuoiRong(const char *s) {
  if (s == nullptr)
    return true;
  while (*s != '\0') {
    if (!isspace((unsigned char)*s))
      return false;
    s++;
  }
  return true;
}

// Chuẩn hóa khoảng trắng: xóa khoảng trắng đầu/cuối và gộp khoảng trắng ở giữa
void ChuanHoaKhoangTrang(char *s) {
  if (s == nullptr)
    return;

  int i = 0, j = 0;
  while (s[i] != '\0' && isspace((unsigned char)s[i])) {
    i++;
  }

  bool spaceFound = false;
  while (s[i] != '\0') {
    if (isspace((unsigned char)s[i])) {
      if (!spaceFound) {
        s[j++] = ' ';
        spaceFound = true;
      }
    } else {
      s[j++] = s[i];
      spaceFound = false;
    }
    i++;
  }

  if (j > 0 && s[j - 1] == ' ') {
    j--;
  }
  s[j] = '\0';
}

// Định dạng Title Case cho họ tên độc giả
bool DinhDangHoTen(char *s, int sucChua) {
  if (!s || sucChua <= 0) return false;
  try {
    std::wstring w = Chuoi::thuong(s), ketQua;
    bool dauTu = true;
    for (wchar_t c : w) {
      WORD loai = 0;
      GetStringTypeW(CT_CTYPE1, &c, 1, &loai);
      if (loai & C1_SPACE) {
        if (!ketQua.empty() && ketQua.back() != L' ') ketQua += L' ';
        dauTu = true;
      } else {
        if (dauTu) CharUpperBuffW(&c, 1);
        ketQua += c;
        dauTu = false;
      }
    }
    if (!ketQua.empty() && ketQua.back() == L' ') ketQua.pop_back();
    std::string result = Chuoi::utf8(ketQua);
    if (result.size() >= static_cast<unsigned>(sucChua)) return false;
    std::strcpy(s, result.c_str());
    return true;
  } catch (const std::invalid_argument &) { return false; }
}

// Chuẩn hóa mã sang chữ in hoa không khoảng trắng
void ChuanHoaMa(char *s) {
  if (s == nullptr)
    return;

  int i = 0, j = 0;
  while (s[i] != '\0') {
    if (!isspace((unsigned char)s[i])) {
      s[j++] = (char)toupper((unsigned char)s[i]);
    }
    i++;
  }
  s[j] = '\0';
}

// Kiểm tra lỗi họ tên (chỉ nhận chữ cái và khoảng trắng)
// Trả về: 0 = hợp lệ, 1 = rỗng, 2 = có số, 3 = có ký tự đặc biệt, 4 = không có chữ cái
int KiemTraLoiHoTen(const char *s, char &kyTuViPham) {
  kyTuViPham = '\0';
  if (!s || KiemTraChuoiRong(s)) return 1;
  try {
    bool coChu = false;
    for (wchar_t c : Chuoi::rong(s)) {
      WORD loai = 0;
      GetStringTypeW(CT_CTYPE1, &c, 1, &loai);
      if (loai & C1_ALPHA) coChu = true;
      else if (loai & C1_SPACE) continue;
      else {
        kyTuViPham = c < 128 ? static_cast<char>(c) : '?';
        return (loai & C1_DIGIT) ? 2 : 3;
      }
    }
    return coChu ? 0 : 4;
  } catch (const std::invalid_argument &) { return 3; }
}

// Kiểm tra họ tên hợp lệ
bool KiemTraChuCaiVaKhoangTrang(const char *s) {
  char dummy = '\0';
  return (KiemTraLoiHoTen(s, dummy) == 0);
}

// Kiểm tra định dạng mã sách [ISBN]_[STT]
bool KiemTraDinhDangMaSach(const char *maSach) {
  if (maSach == nullptr || maSach[0] == '\0')
    return false;

  const char *underscore = strchr(maSach, '_');
  if (underscore == nullptr || underscore == maSach)
    return false;

  for (const char *p = maSach; p < underscore; p++) {
    if (isspace((unsigned char)*p))
      return false;
  }

  const char *sttStr = underscore + 1;
  if (*sttStr == '\0')
    return false;

  for (int i = 0; sttStr[i] != '\0'; i++) {
    if (!isdigit((unsigned char)sttStr[i]))
      return false;
  }

  int stt = 0;
  for (const char *p = sttStr; *p; ++p) {
    if (stt > (2147483647 - (*p - '0')) / 10) return false;
    stt = stt * 10 + (*p - '0');
  }
  return stt > 0 && strlen(maSach) < sizeof(MuonTra::maSach);
}

// So khớp chuỗi con không phân biệt hoa thường
bool ChuaChuoiConKhongPhanBietHoaThuong(const char *str, const char *subStr) {
  if (!str || !subStr) return false;
  try { return Chuoi::thuong(str).find(Chuoi::thuong(subStr)) != std::wstring::npos; }
  catch (const std::invalid_argument &) { return false; }
}

// Khởi tạo cây BST
void KhoiTaoCay(TREE_DocGia &root) { root = nullptr; }

// Kiểm tra cây rỗng
bool KiemTraCayRong(TREE_DocGia root) { return root == nullptr; }

// Đếm tổng số lượng độc giả (đệ quy)
int DemSoLuongDocGia(TREE_DocGia root) {
  if (root == nullptr)
    return 0;
  return 1 + DemSoLuongDocGia(root->pLeft) + DemSoLuongDocGia(root->pRight);
}

// Giải phóng DSLK mượn trả của một độc giả
void GiaiPhongDSMuonTra(NodeMuonTra *&dsMuonTra) {
  NodeMuonTra *pCurr = dsMuonTra;
  while (pCurr != nullptr) {
    NodeMuonTra *pNext = pCurr->pNext;
    delete pCurr;
    pCurr = pNext;
  }
  dsMuonTra = nullptr;
}

// Giải phóng toàn bộ bộ nhớ cây BST theo hậu thứ tự (LRN)
void GiaiPhongCayDocGia(TREE_DocGia &root) {
  if (root == nullptr)
    return;
  GiaiPhongCayDocGia(root->pLeft);
  GiaiPhongCayDocGia(root->pRight);
  GiaiPhongDSMuonTra(root->data.dsMuonTra);
  delete root;
  root = nullptr;
}

// Duyệt cây BST tìm kiếm độc giả theo từ khóa
static void DuyetLNR_TimDocGia(TREE_DocGia root, const char *tuKhoa,
                               TheDocGia *arr[], int &count, int maxKetQua) {
  if (root == nullptr || count >= maxKetQua)
    return;

  DuyetLNR_TimDocGia(root->pLeft, tuKhoa, arr, count, maxKetQua);

  if (count < maxKetQua) {
    char hoTen[75];
    snprintf(hoTen, sizeof(hoTen), "%s %s", root->data.ho, root->data.ten);

    if (ChuaChuoiConKhongPhanBietHoaThuong(root->data.ten, tuKhoa) ||
        ChuaChuoiConKhongPhanBietHoaThuong(root->data.ho, tuKhoa) ||
        ChuaChuoiConKhongPhanBietHoaThuong(hoTen, tuKhoa)) {
      arr[count++] = &(root->data);
    }
  }

  DuyetLNR_TimDocGia(root->pRight, tuKhoa, arr, count, maxKetQua);
}

// Tìm kiếm độc giả theo tên hoặc họ tên
int TimKiemDocGiaTheoTen(TREE_DocGia root, const char *tuKhoa, TheDocGia *arr[],
                         int maxKetQua) {
  int count = 0;
  if (root == nullptr || tuKhoa == nullptr || arr == nullptr || maxKetQua <= 0)
    return 0;

  char tuKhoaChuan[100];
  strncpy(tuKhoaChuan, tuKhoa, sizeof(tuKhoaChuan) - 1);
  tuKhoaChuan[sizeof(tuKhoaChuan) - 1] = '\0';
  ChuanHoaKhoangTrang(tuKhoaChuan);

  if (tuKhoaChuan[0] == '\0')
    return 0;

  DuyetLNR_TimDocGia(root, tuKhoaChuan, arr, count, maxKetQua);
  return count;
}
