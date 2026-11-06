#include "CauTruc.h"
#include "DanhMucSach.h"
#include "Date.h"
#include "DauSach.h"
#include "DocGia.h"
#include "MuonTra.h"
#include "UI.h"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <limits>
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

// ============================================================================
// ĐỒ ÁN CẤU TRÚC DỮ LIỆU & GIẢI THUẬT: QUẢN LÝ THƯ VIỆN (C++)
// FILE DÙNG CHUNG: main.cpp — ĐIỂM KHỞI CHẠY, MENU ĐIỀU HƯỚNG & QUẢN LÝ BỘ NHỚ
// NHÓM THỰC HIỆN:
//   - Lê Văn Hoàng Hiếu (N24DECE018) — Thẻ Độc Giả & Mượn Trả
//   - Lê Anh Đức        (N24DECE012) — Đầu Sách, Danh Mục Sách, Date & Core UI
// ============================================================================

// Đường dẫn file lưu trữ chuẩn của dự án
const char *FILE_DOCGIA  = "Data/DocGia.txt";
const char *FILE_DAUSACH = "Data/DauSach.txt";

static TheDocGia **dsBangDocGia = nullptr;
static void LayODocGia(int dong, int cot, char *buf, int maxLen) {
  const TheDocGia *dg = dsBangDocGia[dong];
  switch (cot) {
  case 0: snprintf(buf, maxLen, "%d", dong + 1); break;
  case 1: snprintf(buf, maxLen, "%d", dg->maThe); break;
  case 2: snprintf(buf, maxLen, "%s %s", dg->ho, dg->ten); break;
  case 3: snprintf(buf, maxLen, "%s", dg->phai); break;
  default: snprintf(buf, maxLen, "%s", dg->trangThai == 1 ? "Hoat dong" : "Bi khoa");
  }
}

// Mảng chỉ mượn địa chỉ trong cây; duyệt LNR giữ thứ tự mã và không sao chép dữ liệu.
static void BangDocGia(TREE_DocGia root, bool theoTen) {
  int n = DemSoLuongDocGia(root);
  if (n == 0) { cout << "[THONG BAO] Khong co du lieu de hien thi.\n"; return; }
  dsBangDocGia = new TheDocGia*[n];
  int index = 0;
  DoCayVaoMang(root, dsBangDocGia, index);
  if (theoTen && n > 1) QuickSortDocGia(dsBangDocGia, 0, n - 1);
  const CauHinhCot cot[] = {{"STT", 4}, {"MA THE", 8}, {"HO VA TEN", 32},
                            {"PHAI", 5}, {"TRANG THAI", 12}};
  HienThiBangPhanTrang("DANH SACH DOC GIA", cot, 5, LayODocGia, n);
  delete[] dsBangDocGia;
  dsBangDocGia = nullptr;
}

// ============================================================================
// CÁC HÀM TIỆN ÍCH GIAO DIỆN & NHẬP LIỆU AN TOÀN
// ============================================================================

void XoaManHinh() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

void DungManHinh() {
  cout << "\nNhan Enter de tiep tuc...";
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void XoaBoDemCin() {
  cin.clear();
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// [HÀM NHẬP CHUỖI AN TOÀN CHỐNG TRÀN BỘ ĐỆM]
// Đọc chuỗi tối đa maxLen ký tự (kể cả '\0'). Nếu người dùng gõ vượt quá độ dài,
// tự động dọn dẹp cờ failbit và loại bỏ ký tự thừa còn lại trong luồng cin.
void NhapChuoiAnToan(char *buf, int maxLen, const char *prompt = nullptr) {
  if (prompt != nullptr && prompt[0] != '\0') {
    cout << prompt;
  }
  cin.getline(buf, maxLen);
  if (cin.fail()) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "  [CANH BAO] Do dai nhap vuot qua toi da " << (maxLen - 1)
         << " ky tu, he thong da tu dong cat ngan chuoi!\n";
  }
}

// [HÀM NHẬP SỐ NGUYÊN AN TOÀN]
// Bẫy lỗi nhập ký tự chữ/dấu, kiểm tra giới hạn minVal - maxVal, chống trôi lệnh.
bool NhapSoNguyenAnToan(int &val, int minVal, int maxVal, const char *prompt) {
  cout << prompt;
  if (!(cin >> val)) {
    // Giữ cờ EOF để vòng nhập lại có thể dừng khi luồng đầu vào đóng.
    if (cin.eof()) return false;
    XoaBoDemCin();
    cout << "[LOI] Du lieu nhap vao phai la so nguyen!\n";
    return false;
  }
  XoaBoDemCin();
  if (val < minVal || val > maxVal) {
    cout << "[LOI] Gia tri phai nam trong khoang tu " << minVal << " den "
         << maxVal << "!\n";
    return false;
  }
  return true;
}

// [HÀM XÁC NHẬN HÀNH ĐỘNG]
// Hỏi người dùng xác nhận thao tác quan trọng (thêm, sửa, xóa, mượn, trả).
bool XacNhanHanhDong(const char *prompt) {
  cout << prompt << " (Y/N): ";
  char c = 'N';
  cin >> c;
  XoaBoDemCin();
  return (c == 'Y' || c == 'y');
}

// ============================================================================
// 1. NHÓM CHỨC NĂNG QUẢN LÝ THẺ ĐỘC GIẢ (HIẾU PHỤ TRÁCH)
// ============================================================================

void ChucNang_ThemDocGia(TREE_DocGia &root) {
  cout << "\n--- [A]. THEM THE DOC GIA MOI ---\n";
  TheDocGia dg;
  dg.maThe = SinhMaTheTuDong(root);
  cout << "-> Ma the tu dong cap: " << dg.maThe << "\n";

  // Nhập họ và tên đệm với loop-until-valid (chặn số 0-9, chặn ký tự đặc biệt)
  while (true) {
    cout << "Nhap ho va ten dem (Enter de huy): ";
    NhapChuoiAnToan(dg.ho, sizeof(dg.ho));
    if (strlen(dg.ho) == 0) {
      cout << "Da huy thao tac them doc gia.\n";
      return;
    }
    char kyTuLoi = '\0';
    int maLoi = KiemTraLoiHoTen(dg.ho, kyTuLoi);
    if (maLoi == 1) {
      cout << "  [LOI] Ho va ten dem khong duoc de trong! Vui long nhap lai.\n";
      continue;
    }
    if (maLoi == 2) {
      cout << "  [LOI] Ho va ten dem chi duoc chua chu cai, khong duoc chua chu so '"
           << kyTuLoi << "'!\n  Vui long nhap lai.\n";
      continue;
    }
    if (maLoi == 3) {
      cout << "  [LOI] Ho va ten dem khong duoc chua ky tu dac biet '"
           << kyTuLoi << "'!\n  Vui long nhap lai.\n";
      continue;
    }
    if (maLoi == 4) {
      cout << "  [LOI] Ho va ten dem phai chua it nhat mot chu cai! Vui long nhap lai.\n";
      continue;
    }
    DinhDangHoTen(dg.ho);
    break;
  }

  // Nhập tên với loop-until-valid (cho phép tên 2 từ, chặn số 0-9 và ký tự đặc biệt)
  while (true) {
    cout << "Nhap ten: ";
    NhapChuoiAnToan(dg.ten, sizeof(dg.ten));
    char kyTuLoi = '\0';
    int maLoi = KiemTraLoiHoTen(dg.ten, kyTuLoi);
    if (maLoi == 1) {
      cout << "  [LOI] Ten khong duoc de trong! Vui long nhap lai.\n";
      continue;
    }
    if (maLoi == 2) {
      cout << "  [LOI] Ten chi duoc chua chu cai, khong duoc chua chu so '"
           << kyTuLoi << "'!\n  Vui long nhap lai.\n";
      continue;
    }
    if (maLoi == 3) {
      cout << "  [LOI] Ten khong duoc chua ky tu dac biet '"
           << kyTuLoi << "'!\n  Vui long nhap lai.\n";
      continue;
    }
    if (maLoi == 4) {
      cout << "  [LOI] Ten phai chua it nhat mot chu cai! Vui long nhap lai.\n";
      continue;
    }
    DinhDangHoTen(dg.ten);
    break;
  }

  // Chọn phái với bẫy lỗi số nguyên
  int chonPhai = 1;
  if (!NhapSoNguyenAnToan(chonPhai, 1, 2, "Chon phai (1. Nam | 2. Nu): ")) {
    return;
  }
  strcpy(dg.phai, (chonPhai == 2) ? "Nu" : "Nam");

  dg.trangThai = 1; // 1 = Đang hoạt động
  dg.dsMuonTra = nullptr;

  // Hiển thị tóm tắt thông tin và xác nhận trước khi lưu
  cout << "\n----------------------------------------\n";
  cout << "XAC NHAN THONG TIN THE DOC GIA:\n";
  cout << "  - Ma the:   " << dg.maThe << "\n";
  cout << "  - Ho ten:   " << dg.ho << " " << dg.ten << "\n";
  cout << "  - Gioi tinh: " << dg.phai << "\n";
  cout << "  - Trang thai: Hoat dong\n";
  cout << "----------------------------------------\n";

  if (!XacNhanHanhDong("Ban co chac chan muon luu doc gia nay khong?")) {
    cout << "Da huy thao tac them doc gia.\n";
    return;
  }

  if (ThemDocGia(root, dg)) {
    cout << "[THANH CONG] Da them the doc gia: " << dg.ho << " " << dg.ten
         << " (Ma: " << dg.maThe << ")\n";
    // TỰ ĐỘNG LƯU NGAY LẬP TỨC
    GhiFileDocGia(root, FILE_DOCGIA);
    cout << "[HE THONG] Da tu dong luu du lieu vao " << FILE_DOCGIA << "!\n";
  }
}

// [CHỨC NĂNG MỚI]: TRA CỨU ĐỘC GIẢ THEO MÃ THẺ HOẶC TÊN (TÌM KIẾM GẦN ĐÚNG)
void ChucNang_TraCuuDocGia(TREE_DocGia root) {
  cout << "\n--- TRA CUU THONG TIN THE DOC GIA ---\n";
  cout << " [1]. Tra cuu theo MA THE (Chinh xac - O(log N))\n";
  cout << " [2]. Tra cuu theo TEN / HO TEN (Gan dung, khong phan biet hoa thuong)\n";
  int cheDo = 1;
  if (!NhapSoNguyenAnToan(cheDo, 1, 2, "Chon che do tra cuu (1 hoac 2): ")) {
    return;
  }

  if (cheDo == 1) {
    int maThe;
    if (!NhapSoNguyenAnToan(maThe, 1, 999999, "Nhap ma the can tra cuu: ")) {
      return;
    }
    NodeDocGia *pDG = TimKiemDocGia(root, maThe);
    if (!pDG) {
      cout << "[THONG BAO] Khong tim thay doc gia co ma the: " << maThe << "\n";
      return;
    }

    int soSachMuon = DemSoSachDangMuon(pDG->data.dsMuonTra);
    cout << "\n=============================================================\n";
    cout << "             THONG TIN CHI TIET THE DOC GIA                 \n";
    cout << "=============================================================\n";
    printf("  Ma the:        %-10d\n", pDG->data.maThe);
    printf("  Ho va ten:     %s %s\n", pDG->data.ho, pDG->data.ten);
    printf("  Gioi tinh:     %-10s\n", pDG->data.phai);
    printf("  Trang thai:    %s\n",
           (pDG->data.trangThai == 1 ? "Hoat dong" : "Bi khoa"));
    printf("  Sach dang giu: %d / %d cuon\n", soSachMuon, MAX_MUON_SACH);
    cout << "=============================================================\n";
  } else {
    char tuKhoa[100];
    while (true) {
      cout << "Nhap ten hoac ho ten can tim (Enter de huy): ";
      NhapChuoiAnToan(tuKhoa, sizeof(tuKhoa));
      ChuanHoaKhoangTrang(tuKhoa);
      if (strlen(tuKhoa) == 0) {
        cout << "Da huy tra cuu.\n";
        return;
      }
      break;
    }

    TheDocGia *ketQua[500];
    int n = TimKiemDocGiaTheoTen(root, tuKhoa, ketQua, 500);
    if (n <= 0) {
      cout << "[THONG BAO] Khong tim thay doc gia nao co ten chua \"" << tuKhoa
           << "\".\n";
      return;
    }

    cout << "\nTim thay " << n << " doc gia phu hop:\n";
    printf("+-----+--------+------------------------------------+------+------------+------------+\n");
    printf("| STT | Ma The | Ho va Ten                          | Phai | Trang Thai | Dang Muon  |\n");
    printf("+-----+--------+------------------------------------+------+------------+------------+\n");

    for (int i = 0; i < n; i++) {
      char hoTen[75];
      snprintf(hoTen, sizeof(hoTen), "%s %s", ketQua[i]->ho, ketQua[i]->ten);
      // Đếm số sách đang mượn
      int dangMuon = 0;
      NodeMuonTra *pMT = ketQua[i]->dsMuonTra;
      while (pMT) {
        if (pMT->data.trangThai == 0) dangMuon++;
        pMT = pMT->pNext;
      }

      printf("| %-3d | %-6d | %-34.34s | %-4s | %-10s | %d / %-6d |\n",
             i + 1, ketQua[i]->maThe, hoTen, ketQua[i]->phai,
             ketQua[i]->trangThai == 1 ? "Hoat dong" : "Bi khoa",
             dangMuon, MAX_MUON_SACH);
    }
    printf("+-----+--------+------------------------------------+------+------------+------------+\n");
  }
}

void ChucNang_HieuChinhDocGia(TREE_DocGia root) {
  cout << "\n--- [A]. HIEU CHINH THONG TIN DOC GIA ---\n";
  int maThe;
  if (!NhapSoNguyenAnToan(maThe, 1, 999999, "Nhap ma the can hieu chinh: ")) {
    return;
  }

  NodeDocGia *pDG = TimKiemDocGia(root, maThe);
  if (!pDG) {
    cout << "[LOI] Khong tim thay doc gia co ma the: " << maThe << "\n";
    return;
  }

  cout << "Thong tin hien tai: " << pDG->data.ho << " " << pDG->data.ten
       << " | Phai: " << pDG->data.phai << " | Trang thai: "
       << (pDG->data.trangThai == 1 ? "Hoat dong" : "Bi khoa") << "\n\n";

  char hoMoi[50], tenMoi[20], phaiMoi[5];
  int ttMoi = pDG->data.trangThai;

  // Hiệu chỉnh Họ mới với bẫy lỗi chữ số và ký tự đặc biệt
  while (true) {
    cout << "Nhap ho moi (Enter de giu nguyen): ";
    NhapChuoiAnToan(hoMoi, sizeof(hoMoi));
    if (strlen(hoMoi) == 0) {
      strcpy(hoMoi, pDG->data.ho);
      break;
    }
    char kyTuLoi = '\0';
    int maLoi = KiemTraLoiHoTen(hoMoi, kyTuLoi);
    if (maLoi == 2) {
      cout << "  [LOI] Ho moi chi duoc chua chu cai, khong duoc chua chu so '"
           << kyTuLoi << "'!\n  Vui long nhap lai.\n";
      continue;
    }
    if (maLoi == 3) {
      cout << "  [LOI] Ho moi khong duoc chua ky tu dac biet '"
           << kyTuLoi << "'!\n  Vui long nhap lai.\n";
      continue;
    }
    if (maLoi == 4) {
      cout << "  [LOI] Ho moi phai chua it nhat mot chu cai! Vui long nhap lai.\n";
      continue;
    }
    DinhDangHoTen(hoMoi);
    break;
  }

  // Hiệu chỉnh Tên mới với bẫy lỗi chữ số và ký tự đặc biệt (cho phép tên 2 từ)
  while (true) {
    cout << "Nhap ten moi (Enter de giu nguyen): ";
    NhapChuoiAnToan(tenMoi, sizeof(tenMoi));
    if (strlen(tenMoi) == 0) {
      strcpy(tenMoi, pDG->data.ten);
      break;
    }
    char kyTuLoi = '\0';
    int maLoi = KiemTraLoiHoTen(tenMoi, kyTuLoi);
    if (maLoi == 2) {
      cout << "  [LOI] Ten moi chi duoc chua chu cai, khong duoc chua chu so '"
           << kyTuLoi << "'!\n  Vui long nhap lai.\n";
      continue;
    }
    if (maLoi == 3) {
      cout << "  [LOI] Ten moi khong duoc chua ky tu dac biet '"
           << kyTuLoi << "'!\n  Vui long nhap lai.\n";
      continue;
    }
    if (maLoi == 4) {
      cout << "  [LOI] Ten moi phai chua it nhat mot chu cai! Vui long nhap lai.\n";
      continue;
    }
    DinhDangHoTen(tenMoi);
    break;
  }

  int chonPhai;
  if (!NhapSoNguyenAnToan(chonPhai, 0, 2, "Chon phai (1. Nam | 2. Nu | 0. Giu nguyen): ")) {
    return;
  }
  if (chonPhai == 1)
    strcpy(phaiMoi, "Nam");
  else if (chonPhai == 2)
    strcpy(phaiMoi, "Nu");
  else
    strcpy(phaiMoi, pDG->data.phai);

  if (!NhapSoNguyenAnToan(ttMoi, 0, 1, "Trang thai the (1. Hoat dong | 0. Khoa the): ")) {
    return;
  }

  // Tóm tắt và xác nhận
  cout << "\n----------------------------------------\n";
  cout << "THONG TIN SAU KHI HIEU CHINH:\n";
  cout << "  - Ma the:   " << maThe << "\n";
  cout << "  - Ho ten:   " << hoMoi << " " << tenMoi << "\n";
  cout << "  - Gioi tinh: " << phaiMoi << "\n";
  cout << "  - Trang thai: " << (ttMoi == 1 ? "Hoat dong" : "Bi khoa") << "\n";
  cout << "----------------------------------------\n";

  if (!XacNhanHanhDong("Ban co chac chan muon cap nhat thong tin nay?")) {
    cout << "Da huy thao tac hieu chinh.\n";
    return;
  }

  if (HieuChinhDocGia(root, maThe, hoMoi, tenMoi, phaiMoi, ttMoi)) {
    cout << "[THANH CONG] Da cap nhat thong tin doc gia " << maThe << "!\n";
    // TỰ ĐỘNG LƯU NGAY LẬP TỨC
    GhiFileDocGia(root, FILE_DOCGIA);
    cout << "[HE THONG] Da tu dong luu du lieu vao " << FILE_DOCGIA << "!\n";
  }
}

void ChucNang_XoaDocGia(TREE_DocGia &root) {
  cout << "\n--- [A]. XOA THE DOC GIA ---\n";
  int maThe;
  if (!NhapSoNguyenAnToan(maThe, 1, 999999, "Nhap ma the can xoa: ")) {
    return;
  }

  NodeDocGia *pDG = TimKiemDocGia(root, maThe);
  if (!pDG) {
    cout << "[LOI] Khong tim thay doc gia co ma the: " << maThe << "\n";
    return;
  }

  // Cảnh báo nếu độc giả đang giữ sách
  if (KiemTraDocGiaDangMuonSach(pDG)) {
    cout << "[LOI] Doc gia " << pDG->data.ho << " " << pDG->data.ten
         << " (Ma: " << maThe
         << ") dang giu sach chua tra! Khong duoc phep xoa the.\n";
    return;
  }

  cout << "Ban dang chon xoa doc gia: " << pDG->data.ho << " " << pDG->data.ten
       << " (Ma: " << maThe << ")\n";

  if (!XacNhanHanhDong("CANH BAO: Thao tac nay khong the hoan tac! Xac nhan xoa?")) {
    cout << "Da huy thao tac xoa.\n";
    return;
  }

  if (XoaDocGia(root, maThe)) {
    cout << "[THANH CONG] Da xoa doc gia " << maThe << " khoi he thong.\n";
    // TỰ ĐỘNG LƯU NGAY LẬP TỨC
    GhiFileDocGia(root, FILE_DOCGIA);
    cout << "[HE THONG] Da tu dong luu du lieu vao " << FILE_DOCGIA << "!\n";
  }
}

// ============================================================================
// 2. NHÓM CHỨC NĂNG QUẢN LÝ ĐẦU SÁCH & DMS (ĐỨC PHỤ TRÁCH)
// ============================================================================

void ChucNang_ThemDauSach(DanhSachDauSach &dsDS) {
  cout << "\n--- [C]. THEM DAU SACH MOI ---\n";
  if (dauSachDay(dsDS)) {
    cout << "[LOI] Danh sach dau sach da day!\n";
    return;
  }

  DauSach *moi = new DauSach{};
  while (true) {
    NhapChuoiAnToan(moi->ISBN, sizeof(moi->ISBN), "Nhap ISBN: ");
    ChuanHoaMa(moi->ISBN);
    if (KiemTraChuoiRong(moi->ISBN)) cout << "[LOI] ISBN khong duoc de trong!\n";
    else if (strchr(moi->ISBN, '_')) cout << "[LOI] ISBN khong duoc chua '_'!\n";
    else if (timDauSachTheoISBN(dsDS, moi->ISBN)) cout << "[LOI] ISBN da ton tai!\n";
    else break;
  }

  do {
    NhapChuoiAnToan(moi->tenSach, sizeof(moi->tenSach), "Nhap ten sach: ");
    ChuanHoaKhoangTrang(moi->tenSach);
    if (KiemTraChuoiRong(moi->tenSach)) cout << "[LOI] Ten sach khong duoc de trong!\n";
  } while (KiemTraChuoiRong(moi->tenSach));

  while (true) {
    NhapChuoiAnToan(moi->tacGia, sizeof(moi->tacGia), "Nhap tac gia: ");
    char kyTu = '\0';
    if (KiemTraLoiHoTen(moi->tacGia, kyTu) == 0) break;
    cout << "[LOI] Ten tac gia phai la chu cai va khong duoc de trong!\n";
  }
  DinhDangHoTen(moi->tacGia);

  do {
    NhapChuoiAnToan(moi->theLoai, sizeof(moi->theLoai), "Nhap the loai: ");
    ChuanHoaKhoangTrang(moi->theLoai);
    if (!KiemTraChuCaiVaKhoangTrang(moi->theLoai))
      cout << "[LOI] The loai phai chua chu cai!\n";
  } while (!KiemTraChuCaiVaKhoangTrang(moi->theLoai));

  while (!NhapSoNguyenAnToan(moi->soTrang, 1, 100000, "Nhap so trang (1 - 100000): "))
    if (cin.eof()) { delete moi; return; }
  while (!NhapSoNguyenAnToan(moi->namXuatBan, 1000, 2100, "Nhap nam xuat ban (1000 - 2100): "))
    if (cin.eof()) { delete moi; return; }

  moi->dms = nullptr;

  // Xác nhận lưu đầu sách
  cout << "\n----------------------------------------\n";
  cout << "THONG TIN DAU SACH MOI:\n";
  cout << "  - ISBN:     " << moi->ISBN << "\n";
  cout << "  - Ten sach: " << moi->tenSach << "\n";
  cout << "  - Tac gia:  " << moi->tacGia << "\n";
  cout << "  - The loai: " << moi->theLoai << "\n";
  cout << "  - So trang: " << moi->soTrang << " | Nam XB: " << moi->namXuatBan << "\n";
  cout << "----------------------------------------\n";

  if (!XacNhanHanhDong("Ban co chac chan muon them dau sach nay?")) {
    delete moi;
    cout << "Da huy thao tac.\n";
    return;
  }

  if (themDauSach(dsDS, moi)) {
    cout << "[THANH CONG] Da them dau sach \"" << moi->tenSach
         << "\" vao danh sach!\n";

    if (!ghiDauSach(dsDS, FILE_DAUSACH)) cout << "[LOI] Khong luu duoc dau sach!\n";
    if (XacNhanHanhDong("Ban co muon them ban sao sach ngay bay gio khong?")) {
      int soLuong = 1;
      char viTri[50];
      if (NhapSoNguyenAnToan(soLuong, 1, 1000, "Nhap so luong ban sao muon them (1 - 1000): ")) {
        do {
          NhapChuoiAnToan(viTri, sizeof(viTri), "Nhap vi tri luu tru: ");
          ChuanHoaKhoangTrang(viTri);
          if (KiemTraChuoiRong(viTri)) cout << "[LOI] Vi tri khong duoc de trong!\n";
        } while (KiemTraChuoiRong(viTri));
        cout << "Ban sao: " << soLuong << " | Vi tri: " << viTri << '\n';
        if (XacNhanHanhDong("Xac nhan them ban sao?") && themBanSao(*moi, soLuong, viTri)) {
          cout << "[THANH CONG] Da tao tu dong " << soLuong
               << " ban sao sach dang [" << moi->ISBN << "_1 -> " << moi->ISBN
               << "_" << soLuong << "].\n";
        }
      }
    }
    // TỰ ĐỘNG LƯU NGAY LẬP TỨC
    ghiDauSach(dsDS, FILE_DAUSACH);
    cout << "[HE THONG] Da tu dong luu du lieu vao " << FILE_DAUSACH << "!\n";
  } else {
    delete moi;
    cout << "[LOI] Them dau sach that bai!\n";
  }
}

void ChucNang_ThemBanSao(DanhSachDauSach &dsDS) {
  cout << "\n--- [C]. THEM BAN SAO SACH CON VAO DANH MUC SACH ---\n";
  char isbn[15];
  cout << "Nhap ISBN cua dau sach can them ban sao: ";
  cin.getline(isbn, sizeof(isbn));
  ChuanHoaMa(isbn);

  DauSach *pDS = timDauSachTheoISBN(dsDS, isbn);
  if (!pDS) {
    cout << "[LOI] Khong tim thay dau sach co ISBN: " << isbn << "\n";
    return;
  }

  cout << "Dau sach: " << pDS->tenSach
       << " (Hien co: " << demSoLuongDMS(pDS->dms) << " cuon)\n";
  int soLuong;
  char viTri[50];
  if (!NhapSoNguyenAnToan(soLuong, 1, 1000, "Nhap so luong cuon sach muon nhap them (1 - 1000): ")) {
    return;
  }

  do {
    NhapChuoiAnToan(viTri, sizeof(viTri), "Nhap vi tri dat sach tren ke: ");
    ChuanHoaKhoangTrang(viTri);
    if (KiemTraChuoiRong(viTri)) cout << "[LOI] Vi tri khong duoc de trong!\n";
  } while (KiemTraChuoiRong(viTri));
  cout << "Dau sach: " << pDS->tenSach << " | So luong: " << soLuong << " | Vi tri: " << viTri << '\n';
  if (!XacNhanHanhDong("Xac nhan them ban sao?")) return;

  if (themBanSao(*pDS, soLuong, viTri)) {
    cout << "[THANH CONG] Da tao them " << soLuong
         << " ban sao vao danh muc sach!\n";
    // TỰ ĐỘNG LƯU NGAY LẬP TỨC
    ghiDauSach(dsDS, FILE_DAUSACH);
    cout << "[HE THONG] Da tu dong luu du lieu vao " << FILE_DAUSACH << "!\n";
  }
}

void ChucNang_TimKiemSach(const DanhSachDauSach &dsDS) {
  cout << "\n--- [E]. TRA CUU THONG TIN SACH THEO TEN ---\n";
  char tuKhoa[100];
  cout << "Nhap ten sach can tim (chua tu khoa): ";
  cin.getline(tuKhoa, sizeof(tuKhoa));
  ChuanHoaKhoangTrang(tuKhoa);

  if (KiemTraChuoiRong(tuKhoa)) {
    cout << "[LOI] Tu khoa tim kiem khong duoc de trong!\n";
    return;
  }

  DauSach *ketQua[MAX_DAUSACH];
  int soLuong = timDauSachTheoTen(dsDS, tuKhoa, ketQua, dsDS.n);
  if (soLuong <= 0) {
    cout << "[THONG BAO] Khong tim thay dau sach nao phu hop voi tu khoa \""
         << tuKhoa << "\".\n";
    return;
  }

  cout << "\nTim thay " << soLuong << " dau sach:\n";
  for (int i = 0; i < soLuong; ++i) {
    DauSach *d = ketQua[i];
    cout << "---------------------------------------------------------\n";
    cout << "[" << (i + 1) << "] ISBN: " << d->ISBN << " | Ten: " << d->tenSach
         << "\n";
    cout << "    Tac gia: " << d->tacGia << " | Nam XB: " << d->namXuatBan
         << " | The loai: " << d->theLoai << " | So trang: " << d->soTrang
         << "\n";
    cout << "    Danh muc cac ban sao sach (" << demSoLuongDMS(d->dms)
         << " cuon):\n";
    NodeDMS *p = d->dms;
    if (!p) {
      cout << "      (Chua co ban sao nao trong kho)\n";
    }
    while (p) {
      const char *tt = (p->data.trangThai == 0)   ? "Cho muon"
                       : (p->data.trangThai == 1) ? "Da muon"
                                                  : "Thanh ly";
      printf("      - Ma sach: %-12s | Trang thai: %-10s | Vi tri: %s\n",
             p->data.maSach, tt, p->data.viTri);
      p = p->pNext;
    }
  }
}

void ChucNang_InSachTheoTheLoai(const DanhSachDauSach &dsDS) {
  cout << "\n--- [D]. IN DANH SACH DAU SACH THEO TUNG THE LOAI ---\n";
  if (dsDS.n == 0) {
    cout << "[THONG BAO] Thu vien chua co dau sach nao.\n";
    return;
  }

  DauSach *ketQua[MAX_DAUSACH];
  int soLuong = layDauSachTheoTheLoai(dsDS, ketQua, dsDS.n);
  if (soLuong <= 0)
    return;

  string theLoaiHienTai = "";
  for (int i = 0; i < soLuong; ++i) {
    DauSach *d = ketQua[i];
    if (theLoaiHienTai != d->theLoai) {
      theLoaiHienTai = d->theLoai;
      cout << "\n=========================================================\n";
      cout << "THE LOAI: " << theLoaiHienTai << "\n";
      cout << "=========================================================\n";
      printf("%-10s | %-32s | %-18s | %-8s\n", "ISBN", "Ten Sach", "Tac Gia",
             "Nam XB");
      cout << "---------------------------------------------------------\n";
    }
    printf("%-10s | %-32.32s | %-18.18s | %-8d\n", d->ISBN, d->tenSach,
           d->tacGia, d->namXuatBan);
  }
}

// ============================================================================
// 3. NHÓM CHỨC NĂNG GIAO DỊCH MƯỢN TRẢ & THỐNG KÊ (PHỐI HỢP CẢ HAI)
// ============================================================================

void ChucNang_MuonSach(TREE_DocGia root, DanhSachDauSach &dsDS) {
  cout << "\n--- [F]. GIAO DICH MUON SACH ---\n";
  int maThe;
  if (!NhapSoNguyenAnToan(maThe, 1, 999999, "Nhap ma the doc gia: ")) {
    return;
  }

  NodeDocGia *pDG = TimKiemDocGia(root, maThe);
  if (!pDG) {
    cout << "[LOI] Khong tim thay doc gia co ma the: " << maThe << "\n";
    return;
  }

  cout << "Doc gia: " << pDG->data.ho << " " << pDG->data.ten
       << " (Ma: " << maThe << ") | Trang thai: "
       << (pDG->data.trangThai == 1 ? "Hoat dong" : "Bi khoa")
       << " | Dang muon: " << DemSoSachDangMuon(pDG->data.dsMuonTra) << " / " << MAX_MUON_SACH
       << " cuon\n";

  char maSach[20];
  while (true) {
    cout << "Nhap ma sach can muon (VD: CS101_1, Enter de huy): ";
    NhapChuoiAnToan(maSach, sizeof(maSach));
    ChuanHoaMa(maSach);
    if (strlen(maSach) == 0) {
      cout << "Da huy giao dich muon sach.\n";
      return;
    }
    if (!KiemTraDinhDangMaSach(maSach)) {
      cout << "  [LOI] Ma sach \"" << maSach
           << "\" khong dung dinh dang [ISBN]_[STT] (Vi du: CS101_1)!\n  Vui long nhap lai.\n";
      continue;
    }
    break;
  }

  // Tra cứu xem sách có tồn tại và hiển thị trước khi xác nhận
  DauSach *pDS = nullptr;
  NodeDMS *pCuon = nullptr;
  if (timSachTheoMaSach(dsDS, maSach, pDS, pCuon) && pDS && pCuon) {
    cout << "Sach: \"" << pDS->tenSach << "\" (Vi tri: " << pCuon->data.viTri
         << ")\n";
  }

  if (!XacNhanHanhDong("Ban co chac chan muon thuc hien giao dich muon sach nay?")) {
    cout << "Da huy giao dich.\n";
    return;
  }

  if (MuonSach(root, dsDS, maThe, maSach)) {
    // TỰ ĐỘNG LƯU NGAY LẬP TỨC
    GhiFileDocGia(root, FILE_DOCGIA);
    ghiDauSach(dsDS, FILE_DAUSACH);
    cout << "[HE THONG] Da tu dong luu cap nhat giao dich vao file!\n";
  }
}

// Xem trước giúp đối chiếu đúng độc giả và bản sách trước khi thay đổi giao dịch.
static bool XemTruocTraSach(const NodeDocGia *docGia, DanhSachDauSach &dsDS,
                           const char *maSach) {
  DauSach *dauSach = nullptr;
  NodeDMS *cuon = nullptr;
  if (!docGia || !timSachTheoMaSach(dsDS, maSach, dauSach, cuon) || !dauSach || !cuon) {
    cout << "[LOI] Khong tim thay cuon sach.\n";
    return false;
  }
  cout << "\n  XEM TRUOC GIAO DICH\n  Doc gia: " << docGia->data.ho << ' ' << docGia->data.ten
       << " | Dang giu: " << DemSoSachDangMuon(docGia->data.dsMuonTra) << " / " << MAX_MUON_SACH
       << "\n  Sach: " << dauSach->tenSach << " | Ma: " << maSach
       << "\n  Vi tri: " << cuon->data.viTri << '\n';
  return true;
}

void ChucNang_TraSach(TREE_DocGia root, DanhSachDauSach &dsDS) {
  cout << "\n--- [G]. GIAO DICH TRA SACH ---\n";
  int maThe;
  if (!NhapSoNguyenAnToan(maThe, 1, 999999, "Nhap ma the doc gia tra sach: ")) {
    return;
  }

  NodeDocGia *pDG = TimKiemDocGia(root, maThe);
  if (!pDG) {
    cout << "[LOI] Khong tim thay doc gia co ma the: " << maThe << "\n";
    return;
  }

  cout << "Doc gia: " << pDG->data.ho << " " << pDG->data.ten
       << " (Ma: " << maThe << ")\n";

  char maSach[20];
  while (true) {
    cout << "Nhap ma sach can tra (VD: CS101_1, Enter de huy): ";
    NhapChuoiAnToan(maSach, sizeof(maSach));
    ChuanHoaMa(maSach);
    if (strlen(maSach) == 0) {
      cout << "Da huy giao dich tra sach.\n";
      return;
    }
    if (!KiemTraDinhDangMaSach(maSach)) {
      cout << "  [LOI] Ma sach \"" << maSach
           << "\" khong dung dinh dang [ISBN]_[STT] (Vi du: CS101_1)!\n  Vui long nhap lai.\n";
      continue;
    }
    break;
  }

  if (!XemTruocTraSach(pDG, dsDS, maSach)) return;
  if (!XacNhanHanhDong("Xac nhan tra cuon sach nay ve thu vien?")) {
    cout << "Da huy giao dich.\n";
    return;
  }

  if (TraSach(root, dsDS, maThe, maSach)) {
    // TỰ ĐỘNG LƯU NGAY LẬP TỨC
    GhiFileDocGia(root, FILE_DOCGIA);
    ghiDauSach(dsDS, FILE_DAUSACH);
    cout << "[HE THONG] Da tu dong luu cap nhat giao dich vao file!\n";
  }
}

void ChucNang_BaoMatSach(TREE_DocGia root, DanhSachDauSach &dsDS) {
  cout << "\n--- [G]. BAO MAT SACH (THANH LY) ---\n";
  int maThe;
  if (!NhapSoNguyenAnToan(maThe, 1, 999999, "Nhap ma the doc gia: ")) {
    return;
  }

  NodeDocGia *pDG = TimKiemDocGia(root, maThe);
  if (!pDG) {
    cout << "[LOI] Khong tim thay doc gia co ma the: " << maThe << "\n";
    return;
  }

  cout << "Doc gia: " << pDG->data.ho << " " << pDG->data.ten
       << " (Ma: " << maThe << ")\n";

  char maSach[20];
  while (true) {
    cout << "Nhap ma sach bi mat (VD: CS101_1, Enter de huy): ";
    NhapChuoiAnToan(maSach, sizeof(maSach));
    ChuanHoaMa(maSach);
    if (strlen(maSach) == 0) {
      cout << "Da huy thao tac bao mat sach.\n";
      return;
    }
    if (!KiemTraDinhDangMaSach(maSach)) {
      cout << "  [LOI] Ma sach \"" << maSach
           << "\" khong dung dinh dang [ISBN]_[STT] (Vi du: CS101_1)!\n  Vui long nhap lai.\n";
      continue;
    }
    break;
  }

  if (!XemTruocTraSach(pDG, dsDS, maSach)) return;
  cout << "[CANH BAO] Sach se chuyen sang trang thai THANH LY va khong the cho muon tiep!\n";
  if (!XacNhanHanhDong("Ban co chac chan muon bao mat va thanh ly cuon sach nay?")) {
    cout << "Da huy thao tac.\n";
    return;
  }

  if (BaoMatSach(root, dsDS, maThe, maSach)) {
    // TỰ ĐỘNG LƯU NGAY LẬP TỨC
    GhiFileDocGia(root, FILE_DOCGIA);
    ghiDauSach(dsDS, FILE_DAUSACH);
    cout << "[HE THONG] Da tu dong luu cap nhat giao dich vao file!\n";
  }
}

void ChucNang_LietKeSachDangMuon(TREE_DocGia root,
                                 const DanhSachDauSach &dsDS) {
  cout << "\n--- [H]. LIET KE SACH DOC GIA DANG MUON ---\n";
  int maThe;
  if (!NhapSoNguyenAnToan(maThe, 1, 999999, "Nhap ma the doc gia: ")) {
    return;
  }

  LietKeSachDangMuon(root, dsDS, maThe);
}

void ChucNang_Top10(const DanhSachDauSach &dsDS, const NodeDocGia *root) {
  cout << "\n--- [J]. TOP 10 DAU SACH CO LUOT MUON NHIEU NHAT ---\n";
  if (dsDS.n == 0) {
    cout << "[THONG BAO] Thu vien chua co dau sach nao.\n";
    return;
  }

  ThongKeSach top10[10];
  int n = layTop10(dsDS, root, top10, 10);
  if (n <= 0) {
    cout << "[THONG BAO] Chua co du lieu giao dich de thong ke.\n";
    return;
  }

  printf(
      "+-----+------------+----------------------------------+------------+\n");
  printf(
      "| TOP |   ISBN     |             Ten Sach             | Luot Muon  |\n");
  printf(
      "+-----+------------+----------------------------------+------------+\n");
  for (int i = 0; i < n; ++i) {
    printf("| %-3d | %-10s | %-32.32s | %-10d |\n", i + 1, top10[i].sach->ISBN,
           top10[i].sach->tenSach, top10[i].soLuotMuon);
  }
  printf(
      "+-----+------------+----------------------------------+------------+\n");
}

// ============================================================================
// HÀM MAIN CHÍNH — KHỞI CHẠY & ĐIỀU HƯỚNG CHƯƠNG TRÌNH
// ============================================================================

int main() {
  // Đảm bảo thư mục lưu trữ Data luôn tồn tại ngay khi bật app
#ifdef _WIN32
  CreateDirectoryA("Data", NULL);
#endif

  TREE_DocGia root;
  KhoiTaoCay(root);

  DanhSachDauSach dsDS;
  khoiTaoDSDS(dsDS);

  cout << "Dang nap du lieu tu dia...\n";
  bool docDGOk = DocFileDocGia(root, FILE_DOCGIA);
  bool docDSOk = docDauSach(dsDS, FILE_DAUSACH);

  // Không ghi đè dữ liệu khi đọc lỗi: file có thể tồn tại nhưng bị hỏng.
  if (!docDGOk || !docDSOk) {
    cout << "[LOI] Khong the doc du lieu. Kiem tra file trong thu muc Data.\n";
    GiaiPhongCayDocGia(root);
    giaiPhongDauSach(dsDS);
    return 1;
  }

  cout << "Khoi dong he thong thanh cong! (Hien co: "
       << DemSoLuongDocGia(root) << " doc gia, "
       << dsDS.n << " dau sach trong kho)\n";

  int luaChon = -1;
  do {
    const MenuItem menu[] = {
      {"DOC GIA", -1}, {"a  Quan ly the doc gia", 1},
      {"b  Danh sach doc gia", 2}, {"SACH", -1},
      {"c  Them dau sach va ban sao", 3}, {"d  Danh sach theo the loai", 4},
      {"e  Tim sach theo ten", 5}, {"MUON TRA & THONG KE", -1},
      {"f  Muon sach", 6}, {"g  Tra sach / Bao mat", 7},
      {"h  Sach doc gia dang muon", 8}, {"i  Doc gia muon qua han", 9},
      {"j  Top 10 sach duoc muon", 10}, {"0  Luu va thoat", 0}
    };
    luaChon = HienThiMenu("MENU CHINH", menu, sizeof(menu) / sizeof(menu[0]));
    if (luaChon == 0 && !XacNhanHanhDong("Ban co muon luu va thoat?")) {
      luaChon = -2;
      continue;
    }
    if (luaChon == 1) {
      const MenuItem sub[] = {{"1  Them doc gia", 1}, {"2  Hieu chinh", 5},
                              {"3  Xoa doc gia", 6}, {"4  Tra cuu", 4}, {"0  Quay lai", 0}};
      luaChon = HienThiMenu("QUAN LY DOC GIA", sub, 5);
    } else if (luaChon == 2) {
      const MenuItem sub[] = {{"1  Theo ma the", 2}, {"2  Theo ten va ho", 3}, {"0  Quay lai", 0}};
      luaChon = HienThiMenu("DANH SACH DOC GIA", sub, 3);
    } else if (luaChon == 3) {
      const MenuItem sub[] = {{"1  Them dau sach", 7}, {"2  Them ban sao", 8}, {"0  Quay lai", 0}};
      luaChon = HienThiMenu("THEM SACH", sub, 3);
    } else if (luaChon == 7) {
      const MenuItem sub[] = {{"1  Tra sach", 12}, {"2  Bao mat", 13}, {"0  Quay lai", 0}};
      luaChon = HienThiMenu("TRA SACH", sub, 3);
    } else if (luaChon > 3) {
      const int map[] = {0, 0, 0, 0, 9, 10, 11, 0, 14, 15, 16};
      luaChon = map[luaChon];
    } else if (luaChon == 0) {
      luaChon = -1; // Đánh dấu thoát đã xác nhận, tách khỏi quay lại menu con.
    }
    if (luaChon == 0) { luaChon = -2; continue; }
    if (luaChon == -1) luaChon = 0;
    XoaManHinhUI();

    switch (luaChon) {
    case 1:
      ChucNang_ThemDocGia(root);
      break;
    case 2:
      BangDocGia(root, false);
      break;
    case 3:
      BangDocGia(root, true);
      break;
    case 4:
      ChucNang_TraCuuDocGia(root);
      break;
    case 5:
      ChucNang_HieuChinhDocGia(root);
      break;
    case 6:
      ChucNang_XoaDocGia(root);
      break;
    case 7:
      ChucNang_ThemDauSach(dsDS);
      break;
    case 8:
      ChucNang_ThemBanSao(dsDS);
      break;
    case 9:
      ChucNang_InSachTheoTheLoai(dsDS);
      break;
    case 10:
      ChucNang_TimKiemSach(dsDS);
      break;
    case 11:
      ChucNang_MuonSach(root, dsDS);
      break;
    case 12:
      ChucNang_TraSach(root, dsDS);
      break;
    case 13:
      ChucNang_BaoMatSach(root, dsDS);
      break;
    case 14:
      ChucNang_LietKeSachDangMuon(root, dsDS);
      break;
    case 15:
      InDanhSachDocGiaQuaHan(root);
      break;
    case 16:
      ChucNang_Top10(dsDS, root);
      break;
    case 0:
      cout << "\nDang luu du lieu xuong dia...\n";
      GhiFileDocGia(root, FILE_DOCGIA);
      ghiDauSach(dsDS, FILE_DAUSACH);
      cout << "Da luu thanh cong file " << FILE_DOCGIA << " va " << FILE_DAUSACH
           << "!\n";
      break;
    default:
      cout << "[LOI] Lua chon khong hop le!\n";
      break;
    }

    if (luaChon != 0) {
      DungManHinh();
    }
  } while (luaChon != 0);

  // Thu hồi toàn bộ bộ nhớ RAM trước khi thoát (Chống Memory Leak)
  cout << "Dang giai phong toan bo bo nho...\n";
  GiaiPhongCayDocGia(root);
  giaiPhongDauSach(dsDS);
  cout << "Cam on ban da su dung chuong trinh! Tam biet.\n";
  return 0;
}
