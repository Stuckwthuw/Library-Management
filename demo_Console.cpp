#include "CauTruc.h"
#include "DocGia.h"
#include "MuonTra.h"
#include "DauSach.h"
#include "DanhMucSach.h"
#include "Date.h"
#include <iostream>
#include <limits>
#include <cstring>
#include <cstdio>

using namespace std;

// ============================================================================
// CHƯƠNG TRÌNH DEMO TƯƠNG TÁC CONSOLE TERMINAL (GIAI ĐOẠN 3)
// Đầy đủ 10 nhóm chức năng nghiệp vụ (từ a đến j theo đề tài)
// Phối hợp đồng bộ giữa phân hệ của Hiếu và phân hệ của Đức
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

// ----------------------------------------------------------------------------
// GIAO DIỆN CÁC CHỨC NĂNG CON
// ----------------------------------------------------------------------------

void Menu_ThemDocGia(TREE_DocGia &root) {
  cout << "\n--- THEM THE DOC GIA MOI ---\n";
  TheDocGia dg;
  dg.maThe = SinhMaTheTuDong(root);
  cout << "-> Ma the tu dong sinh (LCG): " << dg.maThe << "\n";

  cout << "Nhap ho va ten dem: ";
  cin.getline(dg.ho, sizeof(dg.ho));
  if (strlen(dg.ho) == 0) {
    cout << "[LOI] Ho khong duoc de trong!\n";
    return;
  }

  cout << "Nhap ten: ";
  cin.getline(dg.ten, sizeof(dg.ten));
  if (strlen(dg.ten) == 0) {
    cout << "[LOI] Ten khong duoc de trong!\n";
    return;
  }

  int chonPhai = 1;
  cout << "Chon phai (1. Nam | 2. Nu): ";
  cin >> chonPhai;
  XoaBoDemCin();
  strcpy(dg.phai, (chonPhai == 2) ? "Nu" : "Nam");

  dg.trangThai = 1; // 1 = Hoạt động
  dg.dsMuonTra = nullptr;

  if (ThemDocGia(root, dg)) {
    cout << "[THANH CONG] Da them the doc gia: " << dg.ho << " " << dg.ten
         << " (Ma: " << dg.maThe << ")\n";
  }
}

void Menu_HieuChinhDocGia(TREE_DocGia root) {
  cout << "\n--- HIEU CHINH THONG TIN DOC GIA ---\n";
  int maThe;
  cout << "Nhap ma the can hieu chinh: ";
  if (!(cin >> maThe)) {
    XoaBoDemCin();
    cout << "[LOI] Ma the phai la so nguyen!\n";
    return;
  }
  XoaBoDemCin();

  NodeDocGia *pDG = TimKiemDocGia(root, maThe);
  if (!pDG) {
    cout << "[LOI] Khong tim thay doc gia co ma the: " << maThe << "\n";
    return;
  }

  cout << "Thong tin hien tai: " << pDG->data.ho << " " << pDG->data.ten
       << " | Phai: " << pDG->data.phai
       << " | Trang thai: " << (pDG->data.trangThai == 1 ? "Hoat dong" : "Bi khoa")
       << "\n\n";

  char hoMoi[50], tenMoi[20], phaiMoi[5];
  int ttMoi;

  cout << "Nhap ho moi (Enter de giu nguyen): ";
  cin.getline(hoMoi, sizeof(hoMoi));
  if (strlen(hoMoi) == 0) strcpy(hoMoi, pDG->data.ho);

  cout << "Nhap ten moi (Enter de giu nguyen): ";
  cin.getline(tenMoi, sizeof(tenMoi));
  if (strlen(tenMoi) == 0) strcpy(tenMoi, pDG->data.ten);

  cout << "Chon phai (1. Nam | 2. Nu | 0. Giu nguyen): ";
  int chonPhai;
  cin >> chonPhai;
  XoaBoDemCin();
  if (chonPhai == 1) strcpy(phaiMoi, "Nam");
  else if (chonPhai == 2) strcpy(phaiMoi, "Nu");
  else strcpy(phaiMoi, pDG->data.phai);

  cout << "Trang thai the (1. Hoat dong | 0. Khoa the): ";
  cin >> ttMoi;
  XoaBoDemCin();

  if (HieuChinhDocGia(root, maThe, hoMoi, tenMoi, phaiMoi, ttMoi)) {
    cout << "[THANH CONG] Da cap nhat thong tin doc gia " << maThe << "!\n";
  }
}

void Menu_XoaDocGia(TREE_DocGia &root) {
  cout << "\n--- XOA THE DOC GIA ---\n";
  int maThe;
  cout << "Nhap ma the can xoa: ";
  if (!(cin >> maThe)) {
    XoaBoDemCin();
    cout << "[LOI] Ma the phai la so nguyen!\n";
    return;
  }
  XoaBoDemCin();

  cout << "Xac nhan xoa the " << maThe << " (Y/N)? ";
  char cf;
  cin >> cf;
  XoaBoDemCin();
  if (cf == 'Y' || cf == 'y') {
    if (XoaDocGia(root, maThe)) {
      cout << "[THANH CONG] Da xoa doc gia " << maThe << " khoi he thong.\n";
    }
  } else {
    cout << "Da huy thao tac xoa.\n";
  }
}

void Menu_ThemDauSach(DanhSachDauSach &dsDS) {
  cout << "\n--- THEM DAU SACH MOI ---\n";
  if (dauSachDay(dsDS)) {
    cout << "[LOI] Danh sach dau sach da day (MAX = 10000)!\n";
    return;
  }

  DauSach *moi = new DauSach{};
  cout << "Nhap ISBN: ";
  cin.getline(moi->ISBN, sizeof(moi->ISBN));
  if (strlen(moi->ISBN) == 0) {
    delete moi;
    cout << "[LOI] ISBN khong duoc de trong!\n";
    return;
  }

  if (timDauSachTheoISBN(dsDS, moi->ISBN)) {
    delete moi;
    cout << "[LOI] ISBN " << moi->ISBN << " da ton tai trong thu vien!\n";
    return;
  }

  cout << "Nhap ten sach: ";
  cin.getline(moi->tenSach, sizeof(moi->tenSach));
  cout << "Nhap tac gia: ";
  cin.getline(moi->tacGia, sizeof(moi->tacGia));
  cout << "Nhap the loai: ";
  cin.getline(moi->theLoai, sizeof(moi->theLoai));

  cout << "Nhap so trang: ";
  cin >> moi->soTrang;
  cout << "Nhap nam xuat ban: ";
  cin >> moi->namXuatBan;
  XoaBoDemCin();

  moi->dms = nullptr;

  if (themDauSach(dsDS, moi)) {
    cout << "[THANH CONG] Da them dau sach \"" << moi->tenSach << "\" vao danh sach!\n";
    cout << "Ban co muon them ban sao sach ngay bay gio khong (Y/N)? ";
    char c;
    cin >> c;
    XoaBoDemCin();
    if (c == 'Y' || c == 'y') {
      int soLuong;
      char viTri[50];
      cout << "Nhap so luong ban sao muon them: ";
      cin >> soLuong;
      XoaBoDemCin();
      cout << "Nhap vi tri luu tru (VD: Ke A1 - Ngan 2): ";
      cin.getline(viTri, sizeof(viTri));
      if (themBanSao(*moi, soLuong, viTri)) {
        cout << "[THANH CONG] Da tao tu dong " << soLuong << " ban sao sach dang ["
             << moi->ISBN << "_1 -> " << moi->ISBN << "_" << soLuong << "].\n";
      }
    }
  } else {
    delete moi;
    cout << "[LOI] Them dau sach that bai!\n";
  }
}

void Menu_ThemBanSao(DanhSachDauSach &dsDS) {
  cout << "\n--- THEM BAN SAO SACH CON VAO DANH MUC SACH (DMS) ---\n";
  char isbn[15];
  cout << "Nhap ISBN cua dau sach can them ban sao: ";
  cin.getline(isbn, sizeof(isbn));

  DauSach *pDS = timDauSachTheoISBN(dsDS, isbn);
  if (!pDS) {
    cout << "[LOI] Khong tim thay dau sach co ISBN: " << isbn << "\n";
    return;
  }

  cout << "Dau sach: " << pDS->tenSach << " (Hien co: " << demSoLuongDMS(pDS->dms) << " cuon)\n";
  int soLuong;
  char viTri[50];
  cout << "Nhap so luong cuon sach muon nhap them: ";
  cin >> soLuong;
  XoaBoDemCin();
  cout << "Nhap vi tri dat sach tren ke: ";
  cin.getline(viTri, sizeof(viTri));

  if (themBanSao(*pDS, soLuong, viTri)) {
    cout << "[THANH CONG] Da tao them " << soLuong << " ban sao vao danh muc sach!\n";
  }
}

void Menu_TimKiemSach(const DanhSachDauSach &dsDS) {
  cout << "\n--- TRA CUU THONG TIN SACH ---\n";
  char tuKhoa[100];
  cout << "Nhap ten sach can tim (chua tu khoa): ";
  cin.getline(tuKhoa, sizeof(tuKhoa));

  DauSach *ketQua[MAX_DAUSACH];
  int soLuong = timDauSachTheoTen(dsDS, tuKhoa, ketQua, dsDS.n);
  if (soLuong <= 0) {
    cout << "[THONG BAO] Khong tim thay dau sach nao phu hop voi tu khoa \"" << tuKhoa << "\".\n";
    return;
  }

  cout << "\nTim thay " << soLuong << " dau sach:\n";
  for (int i = 0; i < soLuong; ++i) {
    DauSach *d = ketQua[i];
    cout << "---------------------------------------------------------\n";
    cout << "[" << (i + 1) << "] ISBN: " << d->ISBN << " | Ten: " << d->tenSach << "\n";
    cout << "    Tac gia: " << d->tacGia << " | Nam XB: " << d->namXuatBan
         << " | The loai: " << d->theLoai << " | So trang: " << d->soTrang << "\n";
    cout << "    Danh muc cac ban sao sach (" << demSoLuongDMS(d->dms) << " cuon):\n";
    NodeDMS *p = d->dms;
    if (!p) {
      cout << "      (Chua co ban sao nao trong kho)\n";
    }
    while (p) {
      const char *tt = (p->data.trangThai == 0) ? "Cho muon" :
                       (p->data.trangThai == 1) ? "Da muon" : "Thanh ly";
      printf("      - Ma sach: %-12s | Trang thai: %-10s | Vi tri: %s\n",
             p->data.maSach, tt, p->data.viTri);
      p = p->pNext;
    }
  }
}

void Menu_InSachTheoTheLoai(const DanhSachDauSach &dsDS) {
  cout << "\n--- IN DANH SACH DAU SACH THEO THE LOAI ---\n";
  if (dsDS.n == 0) {
    cout << "[THONG BAO] Thu vien chua co dau sach nao.\n";
    return;
  }

  DauSach *ketQua[MAX_DAUSACH];
  int soLuong = layDauSachTheoTheLoai(dsDS, ketQua, dsDS.n);
  if (soLuong <= 0) return;

  string theLoaiHienTai = "";
  for (int i = 0; i < soLuong; ++i) {
    DauSach *d = ketQua[i];
    if (theLoaiHienTai != d->theLoai) {
      theLoaiHienTai = d->theLoai;
      cout << "\n=========================================================\n";
      cout << "THE LOAI: " << theLoaiHienTai << "\n";
      cout << "=========================================================\n";
      printf("%-10s | %-32s | %-18s | %-8s\n", "ISBN", "Ten Sach", "Tac Gia", "Nam XB");
      cout << "---------------------------------------------------------\n";
    }
    printf("%-10s | %-32.32s | %-18.18s | %-8d\n",
           d->ISBN, d->tenSach, d->tacGia, d->namXuatBan);
  }
}

void Menu_MuonSach(TREE_DocGia root, DanhSachDauSach &dsDS) {
  cout << "\n--- GIAO DICH MUON SACH ---\n";
  int maThe;
  char maSach[20];
  cout << "Nhap ma the doc gia: ";
  if (!(cin >> maThe)) {
    XoaBoDemCin();
    cout << "[LOI] Ma the khong hop le!\n";
    return;
  }
  XoaBoDemCin();

  cout << "Nhap ma sach can muon (VD: CS101_1): ";
  cin.getline(maSach, sizeof(maSach));

  MuonSach(root, dsDS, maThe, maSach);
}

void Menu_TraSach(TREE_DocGia root, DanhSachDauSach &dsDS) {
  cout << "\n--- GIAO DICH TRA SACH ---\n";
  int maThe;
  char maSach[20];
  cout << "Nhap ma the doc gia tra sach: ";
  if (!(cin >> maThe)) {
    XoaBoDemCin();
    cout << "[LOI] Ma the khong hop le!\n";
    return;
  }
  XoaBoDemCin();

  cout << "Nhap ma sach can tra: ";
  cin.getline(maSach, sizeof(maSach));

  TraSach(root, dsDS, maThe, maSach);
}

void Menu_BaoMatSach(TREE_DocGia root, DanhSachDauSach &dsDS) {
  cout << "\n--- BAO MAT SACH ---\n";
  int maThe;
  char maSach[20];
  cout << "Nhap ma the doc gia: ";
  if (!(cin >> maThe)) {
    XoaBoDemCin();
    cout << "[LOI] Ma the khong hop le!\n";
    return;
  }
  XoaBoDemCin();

  cout << "Nhap ma sach bi mat: ";
  cin.getline(maSach, sizeof(maSach));

  BaoMatSach(root, dsDS, maThe, maSach);
}

void Menu_LietKeSachDangMuon(TREE_DocGia root, const DanhSachDauSach &dsDS) {
  cout << "\n--- LIET KE SACH DOC GIA DANG MUON ---\n";
  int maThe;
  cout << "Nhap ma the doc gia: ";
  if (!(cin >> maThe)) {
    XoaBoDemCin();
    cout << "[LOI] Ma the khong hop le!\n";
    return;
  }
  XoaBoDemCin();

  LietKeSachDangMuon(root, dsDS, maThe);
}

void Menu_Top10(const DanhSachDauSach &dsDS, const NodeDocGia *root) {
  cout << "\n--- TOP 10 DAU SACH CO LUOT MUON NHIEU NHAT ---\n";
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

  printf("+-----+------------+----------------------------------+------------+\n");
  printf("| TOP |   ISBN     |             Ten Sach             | Luot Muon  |\n");
  printf("+-----+------------+----------------------------------+------------+\n");
  for (int i = 0; i < n; ++i) {
    printf("| %-3d | %-10s | %-32.32s | %-10d |\n",
           i + 1, top10[i].sach->ISBN, top10[i].sach->tenSach, top10[i].soLuotMuon);
  }
  printf("+-----+------------+----------------------------------+------------+\n");
}

// ============================================================================
// HÀM MAIN CHÍNH
// ============================================================================

int main() {
  TREE_DocGia root;
  KhoiTaoCay(root);

  DanhSachDauSach dsDS;
  khoiTaoDSDS(dsDS);

  const char *fileDocGia = "Data/DocGia.txt";
  const char *fileDauSach = "Data/DauSach.txt";

  cout << "Dang nap du lieu tu dia...\n";
  DocFileDocGia(root, fileDocGia);
  docDauSach(dsDS, fileDauSach);
  cout << "Khoi dong he thong thanh cong!\n";

  int luaChon = -1;
  do {
    cout << "\n=============================================================\n";
    cout << "           HE THONG QUAN LY THU VIEN (C++ CTDL&GT)          \n";
    cout << "   Nhom: Le Van Hoang Hieu (Doc Gia) & Le Anh Duc (Dau Sach) \n";
    cout << "=============================================================\n";
    cout << " [1].  Them the doc gia moi (Sinh ma LCG tu dong)\n";
    cout << " [2].  In danh sach doc gia theo MA THE (Duyet BST LNR)\n";
    cout << " [3].  In danh sach doc gia theo TEN + HO (QuickSort Median)\n";
    cout << " [4].  Hieu chinh thong tin the doc gia\n";
    cout << " [5].  Xoa the doc gia khoi cay BST\n";
    cout << "-------------------------------------------------------------\n";
    cout << " [6].  Them dau sach moi (Chen giu thu tu mang con tro)\n";
    cout << " [7].  Them ban sao sach con vao Danh Muc Sach (DMS)\n";
    cout << " [8].  In danh sach dau sach theo tung The Loai\n";
    cout << " [9].  Tra cuu thong tin sach theo Ten / ISBN\n";
    cout << "-------------------------------------------------------------\n";
    cout << " [10]. Giao dich MUON SACH (Bay 3 dieu kien, cap nhat DMS)\n";
    cout << " [11]. Giao dich TRA SACH\n";
    cout << " [12]. Giao dich BAO MAT SACH (Thanh ly)\n";
    cout << " [13]. Liet ke danh sach sach doc gia dang muon\n";
    cout << " [14]. Danh sach doc gia MUON SACH QUA HAN 7 NGAY (QuickSort)\n";
    cout << " [15]. Thong ke TOP 10 dau sach muon nhieu nhat\n";
    cout << "-------------------------------------------------------------\n";
    cout << " [0].  Luu du lieu va Thoat chuong trinh\n";
    cout << "=============================================================\n";
    cout << "Nhap lua chon cua ban (0 - 15): ";

    if (!(cin >> luaChon)) {
      XoaBoDemCin();
      cout << "[LOI] Vui long nhap so nguyen tu 0 den 15!\n";
      DungManHinh();
      continue;
    }
    XoaBoDemCin();

    switch (luaChon) {
    case 1:  Menu_ThemDocGia(root); break;
    case 2:  InDanhSachDocGia_TheoMaThe(root); break;
    case 3:  InDanhSachDocGia_TheoTenHo(root); break;
    case 4:  Menu_HieuChinhDocGia(root); break;
    case 5:  Menu_XoaDocGia(root); break;
    case 6:  Menu_ThemDauSach(dsDS); break;
    case 7:  Menu_ThemBanSao(dsDS); break;
    case 8:  Menu_InSachTheoTheLoai(dsDS); break;
    case 9:  Menu_TimKiemSach(dsDS); break;
    case 10: Menu_MuonSach(root, dsDS); break;
    case 11: Menu_TraSach(root, dsDS); break;
    case 12: Menu_BaoMatSach(root, dsDS); break;
    case 13: Menu_LietKeSachDangMuon(root, dsDS); break;
    case 14: InDanhSachDocGiaQuaHan(root); break;
    case 15: Menu_Top10(dsDS, root); break;
    case 0:
      cout << "\nDang luu du lieu xuong dia...\n";
      GhiFileDocGia(root, fileDocGia);
      ghiDauSach(dsDS, fileDauSach);
      cout << "Da luu thanh cong file " << fileDocGia << " va " << fileDauSach << "!\n";
      break;
    default:
      cout << "[LOI] Lua chon khong hop le! Vui long chon tu 0 den 15.\n";
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
