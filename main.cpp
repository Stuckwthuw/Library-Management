#include "DocGia.h"
#include "DauSach.h"
#include "MuonTra.h"
#include "UI.h"
#include "Common/LuuTru.h"
#include <iostream>
#include <windows.h>
using namespace std;

// Hàm main: khởi chạy và điều hướng chương trình
int main() {
  KhoiTaoConsole();
  // Tạo thư mục Data nếu chưa có
#ifdef _WIN32
  CreateDirectoryA("Data", NULL);
#endif

  TREE_DocGia root;
  KhoiTaoCay(root);

  DanhSachDauSach dsDS;
  khoiTaoDSDS(dsDS);

  string loi;
  if (!NapDuLieu(root, dsDS, FILE_DOCGIA, FILE_DAUSACH, loi)) {
    cerr << "[LOI] " << loi << '\n';
    GiaiPhongCayDocGia(root); giaiPhongDauSach(dsDS); GiaiPhongLichSuLuuTru();
    return 1;
  }
  cout << "Khoi dong he thong thanh cong! (Hien co: "
       << DemSoLuongDocGia(root) << " doc gia, "
       << dsDS.n << " dau sach trong kho)\n";

  int ketQua = 0;
  try {
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
    if (luaChon < 0) break;
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
      luaChon = -1;
    }
    if (luaChon < -1) break;
    if (luaChon == 0) { luaChon = -2; continue; }
    if (luaChon == -1) luaChon = 0;
    XoaManHinhUI();

    switch (luaChon) {
    case 1:
      ChucNang_ThemDocGia(root, dsDS);
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
      ChucNang_HieuChinhDocGia(root, dsDS);
      break;
    case 6:
      ChucNang_XoaDocGia(root, dsDS);
      break;
    case 7:
      ChucNang_ThemDauSach(root, dsDS);
      break;
    case 8:
      ChucNang_ThemBanSao(root, dsDS);
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
      break;
    default:
      cout << "[LOI] Lua chon khong hop le!\n";
      break;
    }

    if (luaChon != 0) {
      DungManHinh();
    }
  } while (luaChon != 0);

  } catch (const HetDuLieuNhap &) {
    cout << "[THONG BAO] Da dong luong nhap.\n";
  } catch (const std::exception &e) {
    cerr << "[LOI] " << e.what() << '\n';
    ketQua = 2;
  }
  if (ketQua == 0) {
    try { LuuSauThayDoi(root, dsDS); }
    catch (const std::exception &e) { cerr << "[LOI] " << e.what() << '\n'; ketQua = 2; }
  }
  GiaiPhongCayDocGia(root);
  giaiPhongDauSach(dsDS);
  GiaiPhongLichSuLuuTru();
  return ketQua;
}
