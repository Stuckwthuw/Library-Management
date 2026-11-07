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

void ChucNang_ThemDauSach(TREE_DocGia root, DanhSachDauSach &dsDS) {
  cout << "\n--- [C]. THEM DAU SACH MOI ---\n";
  if (dauSachDay(dsDS)) {
    cout << "[LOI] Danh sach dau sach da day!\n";
    return;
  }

  unique_ptr<DauSach> tam(new DauSach{});
  DauSach *moi = tam.get();
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
    if (cin.eof()) {  return; }
  while (!NhapSoNguyenAnToan(moi->namXuatBan, 1000, 2100, "Nhap nam xuat ban (1000 - 2100): "))
    if (cin.eof()) {  return; }

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

    cout << "Da huy thao tac.\n";
    return;
  }

  if (themDauSach(dsDS, moi)) {
    tam.release();
    cout << "[THANH CONG] Da them dau sach \"" << moi->tenSach
         << "\" vao danh sach!\n";

    LuuSauThayDoi(root, dsDS);
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
    LuuSauThayDoi(root, dsDS);
  } else {

    cout << "[LOI] Them dau sach that bai!\n";
  }
}

void ChucNang_ThemBanSao(TREE_DocGia root, DanhSachDauSach &dsDS) {
  cout << "\n--- [C]. THEM BAN SAO SACH CON VAO DANH MUC SACH ---\n";
  char isbn[15];
  cout << "Nhap ISBN cua dau sach can them ban sao: ";
  NhapChuoiAnToan(isbn, sizeof(isbn));
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
    LuuSauThayDoi(root, dsDS);
  }
}


namespace {
DauSach **bangSach = nullptr;
const NodeDMS **bangCuon = nullptr;
void laySach(int row, int col, char *buf, int size) {
    const DauSach *d = bangSach[row];
    switch (col) {
    case 0: snprintf(buf,size,"%d",row+1); break;
    case 1: snprintf(buf,size,"%s",d->ISBN); break;
    case 2: snprintf(buf,size,"%s",d->tenSach); break;
    case 3: snprintf(buf,size,"%s",d->tacGia); break;
    case 4: snprintf(buf,size,"%d",d->namXuatBan); break;
    default: snprintf(buf,size,"%s",d->theLoai); break;
    }
}
void layCuon(int row, int col, char *buf, int size) {
    const DanhMucSach &d = bangCuon[row]->data;
    if (col == 0) snprintf(buf,size,"%s",d.maSach);
    else if (col == 1) snprintf(buf,size,"%s", d.trangThai == 0 ? "Cho muon" : d.trangThai == 1 ? "Da muon" : "Thanh ly");
    else snprintf(buf,size,"%s",d.viTri);
}
void hienKetQua(DauSach **a, int n, const char *title) {
    if (n <= 0) { cout << "[THONG BAO] Khong co dau sach phu hop.\n"; return; }
    bangSach = a;
    const CauHinhCot cot[] = {{"STT",4},{"ISBN",14},{"TEN SACH",28},{"TAC GIA",20},{"NAM",4},{"THE LOAI",14}};
    HienThiBangPhanTrang(title, cot, 6, laySach, n);
    bangSach = nullptr;
    for (;;) {
        int chon = 0;
        if (!NhapSoNguyenAnToan(chon,0,n,"Nhap STT xem chi tiet (0 de quay lai): ")) continue;
        if (!chon) return;
        DauSach *d = a[chon-1];
        cout << "\nISBN: " << d->ISBN << "\nTen sach: " << d->tenSach
             << "\nTac gia: " << d->tacGia << "\nThe loai: " << d->theLoai
             << "\nNam: " << d->namXuatBan << " | So trang: " << d->soTrang << '\n';
        int count = demSoLuongDMS(d->dms);
        if (!count) { cout << "[THONG BAO] Chua co ban sao.\n"; continue; }
        unique_ptr<const NodeDMS*[]> cuon(new const NodeDMS*[count]);
        int i=0;
        for (const NodeDMS *p=d->dms; p; p=p->pNext) cuon[i++]=p;
        DungManHinh();
        bangCuon = cuon.get();
        const CauHinhCot cols[] = {{"MA SACH",19},{"TRANG THAI",12},{"VI TRI",30}};
        HienThiBangPhanTrang(d->tenSach, cols, 3, layCuon, count);
        bangCuon = nullptr;
    }
}
}

void ChucNang_TimKiemSach(const DanhSachDauSach &dsDS) {
    char tuKhoa[100];
    NhapChuoiAnToan(tuKhoa,sizeof(tuKhoa),"Nhap ten sach can tim: ");
    ChuanHoaKhoangTrang(tuKhoa);
    if (KiemTraChuoiRong(tuKhoa)) { cout << "[LOI] Tu khoa khong duoc de trong.\n"; return; }
    unique_ptr<DauSach*[]> a(new DauSach*[dsDS.n]);
    int n=timDauSachTheoTen(dsDS,tuKhoa,a.get(),dsDS.n);
    hienKetQua(a.get(),n,"KET QUA TIM SACH");
}

void ChucNang_InSachTheoTheLoai(const DanhSachDauSach &dsDS) {
    unique_ptr<DauSach*[]> a(new DauSach*[dsDS.n]);
    int n=layDauSachTheoTheLoai(dsDS,a.get(),dsDS.n);
    hienKetQua(a.get(),n,"SACH THEO THE LOAI / TEN");
}
