#include "../UI.h"
#include "../DocGia.h"
#include "../DauSach.h"
#include "../DanhMucSach.h"
#include "../MuonTra.h"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <memory>
using namespace std;

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

// Hiển thị bảng danh sách độc giả
void BangDocGia(TREE_DocGia root, bool theoTen) {
  int n = DemSoLuongDocGia(root);
  if (n == 0) { cout << "[THONG BAO] Khong co du lieu de hien thi.\n"; return; }
  unique_ptr<TheDocGia*[]> tam(new TheDocGia*[n]);
  dsBangDocGia = tam.get();
  int index = 0;
  DoCayVaoMang(root, dsBangDocGia, index);
  if (theoTen && n > 1) QuickSortDocGia(dsBangDocGia, 0, n - 1);
  const CauHinhCot cot[] = {{"STT", 4}, {"MA THE", 8}, {"HO VA TEN", 32},
                            {"PHAI", 5}, {"TRANG THAI", 12}};
  HienThiBangPhanTrang("DANH SACH DOC GIA", cot, 5, LayODocGia, n);
  dsBangDocGia = nullptr;
}

void ChucNang_ThemDocGia(TREE_DocGia &root, DanhSachDauSach &dsDS) {
  cout << "\n--- [A]. THEM THE DOC GIA MOI ---\n";
  TheDocGia dg{};
  dg.maThe = SinhMaTheTuDong(root);
  if (dg.maThe < 0) return;
  cout << "-> Ma the tu dong cap: " << dg.maThe << "\n";

  // Nhập họ và tên đệm
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

  // Nhập tên
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

  // Chọn phái
  int chonPhai = 1;
  if (!NhapSoNguyenAnToan(chonPhai, 1, 2, "Chon phai (1. Nam | 2. Nu): ")) {
    return;
  }
  strcpy(dg.phai, (chonPhai == 2) ? "Nu" : "Nam");

  dg.trangThai = 1;
  dg.dsMuonTra = nullptr;

  // Xác nhận trước khi lưu
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
    LuuSauThayDoi(root, dsDS);
  }
}

// Tra cứu độc giả theo mã thẻ hoặc tên
void ChucNang_TraCuuDocGia(TREE_DocGia root) {
  cout << "\n--- TRA CUU THONG TIN THE DOC GIA ---\n";
  cout << " [1]. Tra cuu theo MA THE\n";
  cout << " [2]. Tra cuu theo TEN / HO TEN\n";
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

    int capacity = DemSoLuongDocGia(root);
    unique_ptr<TheDocGia*[]> ketQua(new TheDocGia*[capacity]);
    int n = TimKiemDocGiaTheoTen(root, tuKhoa, ketQua.get(), capacity);
    if (n <= 0) {
      cout << "[THONG BAO] Khong tim thay doc gia nao co ten chua \"" << tuKhoa
           << "\".\n";
      return;
    }

    cout << "\nTim thay " << n << " doc gia phu hop:\n";
    dsBangDocGia = ketQua.get();
    const CauHinhCot cot[] = {{"STT",4},{"MA THE",8},{"HO VA TEN",32},{"PHAI",5},{"TRANG THAI",12}};
    HienThiBangPhanTrang("KET QUA TIM DOC GIA", cot, 5, LayODocGia, n);
    dsBangDocGia = nullptr;

  }
}

void ChucNang_HieuChinhDocGia(TREE_DocGia root, DanhSachDauSach &dsDS) {
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

  // Hiệu chỉnh họ mới
  while (true) {
    cout << "Nhap ho moi (Enter de giu nguyen): ";
    NhapChuoiAnToan(hoMoi, sizeof(hoMoi));
    if (strlen(hoMoi) == 0) {
      strcpy(hoMoi, pDG->data.ho);
      break;
    }
    char kyTuLoi = '\0';
    int maLoi = KiemTraLoiHoTen(hoMoi, kyTuLoi);
    if (maLoi == 1) { cout << "[LOI] Ho ten khong duoc chi co khoang trang.\n"; continue; }
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

  // Hiệu chỉnh tên mới
  while (true) {
    cout << "Nhap ten moi (Enter de giu nguyen): ";
    NhapChuoiAnToan(tenMoi, sizeof(tenMoi));
    if (strlen(tenMoi) == 0) {
      strcpy(tenMoi, pDG->data.ten);
      break;
    }
    char kyTuLoi = '\0';
    int maLoi = KiemTraLoiHoTen(tenMoi, kyTuLoi);
    if (maLoi == 1) { cout << "[LOI] Ho ten khong duoc chi co khoang trang.\n"; continue; }
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

  // Xác nhận cập nhật
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
    LuuSauThayDoi(root, dsDS);
  }
}

void ChucNang_XoaDocGia(TREE_DocGia &root, DanhSachDauSach &dsDS) {
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

  // Kiểm tra độc giả có đang mượn sách không
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
    LuuSauThayDoi(root, dsDS);
  }
}

