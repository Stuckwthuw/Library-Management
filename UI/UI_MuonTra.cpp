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
  } else {
    cout << "[LOI] Khong tim thay cuon sach.\n";
    return;
  }

  if (!XacNhanHanhDong("Ban co chac chan muon thuc hien giao dich muon sach nay?")) {
    cout << "Da huy giao dich.\n";
    return;
  }

  if (MuonSach(root, dsDS, maThe, maSach)) {
    LuuSauThayDoi(root, dsDS);
  }
}

// Xem trước thông tin sách trước khi trả hoặc báo mất
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
    LuuSauThayDoi(root, dsDS);
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
    LuuSauThayDoi(root, dsDS);
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


namespace {
const ThongKeSach *bangTop = nullptr;
const MuonTra **bangDangMuon = nullptr;
const DanhSachDauSach *khoSach = nullptr;
void layTop(int row, int col, char *buf, int size) {
    const ThongKeSach &t=bangTop[row];
    switch (col) {
    case 0: snprintf(buf,size,"%d",row+1); break;
    case 1: snprintf(buf,size,"%s",t.sach->ISBN); break;
    case 2: snprintf(buf,size,"%s",t.sach->tenSach); break;
    default: snprintf(buf,size,"%d",t.soLuotMuon); break;
    }
}
void layDangMuon(int row, int col, char *buf, int size) {
    const MuonTra &m=*bangDangMuon[row];
    if (col==0) snprintf(buf,size,"%s",m.maSach);
    else if (col==1) {
        DauSach *d=nullptr; NodeDMS *cuon=nullptr;
        bool found=timSachTheoMaSach(*khoSach,m.maSach,d,cuon);
        snprintf(buf,size,"%s", found && d ? d->tenSach : "(Khong tim thay)");
    } else snprintf(buf,size,"%02d/%02d/%04d",m.ngayMuon.ngay,m.ngayMuon.thang,m.ngayMuon.nam);
}
}
void ChucNang_Top10(const DanhSachDauSach &ds, const NodeDocGia *root) {
    ThongKeSach a[10];
    int n=layTop10(ds,root,a,10,LayLichSuLuuTru());
    bangTop=a;
    const CauHinhCot cot[]={{"TOP",4},{"ISBN",14},{"TEN SACH",40},{"LUOT MUON",10}};
    HienThiBangPhanTrang("TOP 10 SACH DUOC MUON",cot,4,layTop,n);
    bangTop=nullptr;
}
void LietKeSachDangMuon(TREE_DocGia root, const DanhSachDauSach &ds, int maThe) {
    const MuonTra *a[MAX_MUON_SACH];
    int n=LaySachDangMuon(root,maThe,a,MAX_MUON_SACH);
    if (n<0) { cout << "[LOI] Doc gia hoac so sach dang muon khong hop le.\n"; return; }
    if (!n) { cout << "[THONG BAO] Doc gia khong giu cuon sach nao.\n"; return; }
    bangDangMuon=a; khoSach=&ds;
    const CauHinhCot cot[]={{"MA SACH",19},{"TEN SACH",40},{"NGAY MUON",10}};
    HienThiBangPhanTrang("SACH DANG MUON",cot,3,layDangMuon,n);
    bangDangMuon=nullptr; khoSach=nullptr;
}

namespace {
const QuaHanInfo *bangQuaHan = nullptr;
void layQuaHan(int row, int col, char *buf, int size) {
    const QuaHanInfo &q=bangQuaHan[row];
    switch (col) {
    case 0: snprintf(buf,size,"%d",q.maThe); break;
    case 1: snprintf(buf,size,"%s %s",q.ho,q.ten); break;
    case 2: snprintf(buf,size,"%s",q.maSach); break;
    case 3: snprintf(buf,size,"%02d/%02d/%04d",q.ngayMuon.ngay,q.ngayMuon.thang,q.ngayMuon.nam); break;
    default: snprintf(buf,size,"%d",q.soNgayQuaHan); break;
    }
}
}
void InDanhSachDocGiaQuaHan(TREE_DocGia root) {
    QuaHanInfo *raw = nullptr;
    int n = LapDanhSachQuaHan(root,raw);
    unique_ptr<QuaHanInfo[]> a(raw);
    bangQuaHan = a.get();
    const CauHinhCot cot[]={{"MA THE",8},{"HO VA TEN",30},{"MA SACH",19},{"NGAY MUON",10},{"QUA HAN (NGAY)",14}};
    HienThiBangPhanTrang("DOC GIA QUA HAN",cot,5,layQuaHan,n);
    bangQuaHan = nullptr;
}
