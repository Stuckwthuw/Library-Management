#include "../MuonTra.h"
#include <cstring>
#include <ctime>
#include <iostream>

using namespace std;

// ============================================================================
// PHÂN HỆ: GIAO DỊCH MƯỢN TRẢ (DSLK ĐƠN)
// FILE: MuonTra_Core.cpp — Các hàm phụ trợ nội bộ & Stub tạm thời
// PHỤ TRÁCH: LÊ VĂN HOÀNG HIẾU (N24DECE018)
// ============================================================================

Date LayNgayHienTai_Stub() {
  time_t now = time(0);
  struct tm *ltm = localtime(&now);
  Date d;
  d.ngay = ltm->tm_mday;
  d.thang = ltm->tm_mon + 1;   // tm_mon bắt đầu từ 0
  d.nam = ltm->tm_year + 1900; // tm_year tính từ 1900
  return d;
}

// [STUB] Quy đổi Date sang Julian Day Number
// Mục đích: Chuyển ngày/tháng/năm thành 1 số nguyên duy nhất để trừ cho nhau.
// Giải thuật: Công thức Julian Day Number chuẩn, chính xác với năm nhuận.
//   tất cả quy về 1 phép trừ đơn giản.
static int QuyDoiNgayJulian(Date d) {
  int a = (14 - d.thang) / 12;
  int y = d.nam + 4800 - a;
  int m = d.thang + 12 * a - 3;
  return d.ngay + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 -
         32045;
}

// [STUB] Tính khoảng cách ngày giữa 2 mốc Date (d2 - d1)
// Trả về: Số ngày dương nếu d2 > d1, âm nếu d2 < d1.
int TinhKhoangCachNgay_Stub(Date d1, Date d2) {
  return QuyDoiNgayJulian(d2) - QuyDoiNgayJulian(d1);
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
  Date homNay = LayNgayHienTai_Stub();
  NodeMuonTra *pCurr = dsMuonTra;
  while (pCurr != nullptr) {
    if (pCurr->data.trangThai == 0) {
      int soNgayMuon = TinhKhoangCachNgay_Stub(pCurr->data.ngayMuon, homNay);
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

  Date homNay = LayNgayHienTai_Stub();
  int soSachDangMuon = 0;
  NodeMuonTra *pCurr = pDG->data.dsMuonTra;

  while (pCurr != nullptr) {
    if (pCurr->data.trangThai == 0) { // Sách đang mượn
      soSachDangMuon++;

      // Kiểm tra quá hạn ngay trong cùng vòng lặp
      int soNgayMuon = TinhKhoangCachNgay_Stub(pCurr->data.ngayMuon, homNay);
      if (soNgayMuon > MAX_NGAY_MUON)
        return LOI_CO_SACH_QUA_HAN; // Phát hiện quá hạn → dừng sớm, không cần
                                    // duyệt tiếp
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
