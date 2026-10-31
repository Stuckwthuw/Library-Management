#include "../MuonTra.h"
#include <cstring>
#include <iostream>

using namespace std;

// ============================================================================
// PHÂN HỆ: GIAO DỊCH MƯỢN TRẢ (DSLK ĐƠN)
// FILE: MuonTra_Core.cpp — Các hàm phụ trợ nội bộ & Kết nối module Date của Đức
// PHỤ TRÁCH: LÊ VĂN HOÀNG HIẾU (N24DECE018)
// ============================================================================

// [HÀM LẤY NGÀY HIỆN TẠI — KẾT NỐI CHÍNH THỨC MODULE DATE CỦA ĐỨC]
// Gọi hàm ngayHienTaiHeThong() trong Date/Date_Core.cpp do Đức cài đặt.
Date LayNgayHienTai() {
  return ngayHienTaiHeThong();
}

// [HÀM TÍNH KHOẢNG CÁCH NGÀY — KẾT NỐI CHÍNH THỨC MODULE DATE CỦA ĐỨC]
// Gọi hàm khoangCachNgay(d1, d2) trong Date/Date_KhoangCach.cpp do Đức cài đặt.
// Trả về d2 - d1 (dương nếu d2 > d1).
int TinhKhoangCachNgay(Date d1, Date d2) {
  return khoangCachNgay(d1, d2);
}

// [CÁC HÀM TƯƠNG THÍCH NGƯỢC TỪ GIAI ĐOẠN STUB]
Date LayNgayHienTai_Stub() {
  return LayNgayHienTai();
}

int TinhKhoangCachNgay_Stub(Date d1, Date d2) {
  return TinhKhoangCachNgay(d1, d2);
}

// ############################################################################
// PHẦN 2: CÁC HÀM PHỤ TRỢ CHÍNH THỨC
// ############################################################################

// [HÀM ĐẾM SỐ SÁCH ĐANG MƯỢN CỦA MỘT ĐỘC GIẢ]
// Mục đích: Đếm số node trong DSLK dsMuonTra có trangThai == 0 (đang mượn).
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

// [HÀM KIỂM TRA CÓ SÁCH QUÁ HẠN TRONG DSLK]
// Mục đích: Duyệt DSLK mượn trả, tìm xem có cuốn nào đang mượn (trangThai == 0)
// mà số ngày mượn đã vượt quá MAX_NGAY_MUON (7 ngày).
bool KiemTraCoSachQuaHan(NodeMuonTra *dsMuonTra) {
  Date homNay = LayNgayHienTai();
  NodeMuonTra *pCurr = dsMuonTra;
  while (pCurr != nullptr) {
    if (pCurr->data.trangThai == 0) {
      int soNgayMuon = TinhKhoangCachNgay(pCurr->data.ngayMuon, homNay);
      if (soNgayMuon > MAX_NGAY_MUON)
        return true; // Phát hiện quá hạn → dừng sớm
    }
    pCurr = pCurr->pNext;
  }
  return false;
}

// Mục đích: Kiểm tra cùng lúc 3 điều kiện tiên quyết trước khi cho mượn sách:
//   1. Thẻ đang hoạt động (trangThai == 1)?
//   2. Số sách đang mượn < MAX_MUON_SACH (3)?
//   3. Không có sách nào quá hạn > MAX_NGAY_MUON (7) ngày?
//=============================================================================
//   DU_DIEU_KIEN_MUON: Đủ điều kiện mượn
//   LOI_THE_BI_KHOA: Thẻ bị khóa (trangThai != 1)
//   LOI_DA_MUON_TOI_DA: Đã mượn tối đa MAX_MUON_SACH cuốn
//   LOI_CO_SACH_QUA_HAN: Đang có sách quá hạn > MAX_NGAY_MUON ngày
KetQuaKiemTraMuon KiemTraDieuKienMuon(NodeDocGia *pDG) {
  // ── Điều kiện 1: Thẻ đang hoạt động? ──
  if (pDG->data.trangThai != 1)
    return LOI_THE_BI_KHOA; // Thẻ bị khóa

  Date homNay = LayNgayHienTai();
  int soSachDangMuon = 0;
  NodeMuonTra *pCurr = pDG->data.dsMuonTra;

  while (pCurr != nullptr) {
    if (pCurr->data.trangThai == 0) { // Sách đang mượn
      soSachDangMuon++;

      // Kiểm tra quá hạn ngay trong cùng vòng lặp
      int soNgayMuon = TinhKhoangCachNgay(pCurr->data.ngayMuon, homNay);
      if (soNgayMuon > MAX_NGAY_MUON)
        return LOI_CO_SACH_QUA_HAN; // Phát hiện quá hạn → dừng sớm, không cần duyệt tiếp
    }
    pCurr = pCurr->pNext;
  }

  // Kiểm tra số lượng sau khi duyệt hết DSLK
  if (soSachDangMuon >= MAX_MUON_SACH)
    return LOI_DA_MUON_TOI_DA; // Đã mượn tối đa

  return DU_DIEU_KIEN_MUON; // Đủ điều kiện mượn
}

// [HÀM TÁCH MÃ ISBN TỪ MÃ SÁCH]
// Mục đích: Từ mã sách dạng "[ISBN]_[STT]" (VD: "CS101_2"), tách ra phần ISBN
void TachISBN(const char *maSach, char *isbn) {
  const char *pos = strchr(maSach, '_');
  if (pos != nullptr) {
    int len = (int)(pos - maSach); // Chiều dài phần ISBN
    strncpy(isbn, maSach, len);
    isbn[len] = '\0'; // Kết thúc chuỗi
  } else {
    // Trường hợp biên: maSach không có '_' → copy nguyên chuỗi
    strcpy(isbn, maSach);
  }
}
