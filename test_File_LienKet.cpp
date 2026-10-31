#include "CauTruc.h"
#include "DocGia.h"
#include "MuonTra.h"
#include "DauSach.h"
#include "DanhMucSach.h"
#include <cassert>
#include <cstring>
#include <iostream>

using namespace std;

// ============================================================================
// CHƯƠNG TRÌNH KIỂM THỬ TÍNH BỀN VỮNG DỮ LIỆU LIÊN KẾT (FILE I/O PERSISTENCE)
// Mô phỏng luồng đóng/mở ứng dụng thực tế giữa 2 phiên làm việc (Sessions):
//   Session 1: Mượn sách, cập nhật DMS, ghi file DocGia.txt và DauSach.txt.
//   Giải phóng RAM (giả lập tắt app).
//   Session 2: Đọc file phục hồi lại cây BST và mảng con trỏ, kiểm tra tính
//     đồng bộ trạng thái mượn/trả và DMS, tiếp tục thực hiện giao dịch trả sách.
// ============================================================================

DauSach *TaoDauSach(const char *isbn, const char *ten, const char *theLoai) {
  DauSach *ds = new DauSach{};
  strcpy(ds->ISBN, isbn);
  strcpy(ds->tenSach, ten);
  strcpy(ds->tacGia, "Tac gia");
  strcpy(ds->theLoai, theLoai);
  ds->soTrang = 300;
  ds->namXuatBan = 2026;
  ds->dms = nullptr;
  return ds;
}

int main() {
  cout << "=========================================================\n";
  cout << "=== KIEM THU TINH DONG BO FILE I/O LIEN KET (PERSISTENCE) ===\n";
  cout << "=========================================================\n\n";

  const char *fileDG = "Data/test_docgia_sync.txt";
  const char *fileDS = "Data/test_dausach_sync.txt";

  // ##########################################################################
  // PHIÊN 1 (SESSION 1): KHỞI TẠO, GIAO DỊCH VÀ LƯU FILE
  // ##########################################################################
  cout << ">>> [PHIEN 1] Khoi tao du lieu moi, muon sach va luu vao file...\n";

  TREE_DocGia root1;
  KhoiTaoCay(root1);
  DanhSachDauSach dsDS1;
  khoiTaoDSDS(dsDS1);

  // 1. Nạp 2 đầu sách
  DauSach *s1 = TaoDauSach("CS101", "Cau Truc Du Lieu", "CNTT");
  DauSach *s2 = TaoDauSach("IT002", "Lap Trinh C++", "CNTT");
  themDauSach(dsDS1, s1);
  themDauSach(dsDS1, s2);
  themBanSao(*s1, 2, "Ke A - Ngan 1"); // CS101_1, CS101_2
  themBanSao(*s2, 2, "Ke B - Ngan 2"); // IT002_1, IT002_2

  // 2. Nạp độc giả
  TheDocGia dgA;
  dgA.maThe = 20001;
  strcpy(dgA.ho, "Le Van");
  strcpy(dgA.ten, "An");
  strcpy(dgA.phai, "Nam");
  dgA.trangThai = 1;
  dgA.dsMuonTra = nullptr;
  ThemDocGia(root1, dgA);

  TheDocGia dgB;
  dgB.maThe = 20002;
  strcpy(dgB.ho, "Pham Thi");
  strcpy(dgB.ten, "Binh");
  strcpy(dgB.phai, "Nu");
  dgB.trangThai = 1;
  dgB.dsMuonTra = nullptr;
  ThemDocGia(root1, dgB);

  // 3. Độc giả An (20001) mượn CS101_1
  cout << "1. An (20001) muon cuon CS101_1:\n";
  assert(MuonSach(root1, dsDS1, 20001, "CS101_1") == true);

  // 4. Độc giả Binh (20002) mượn IT002_1
  cout << "2. Binh (20002) muon cuon IT002_1:\n";
  assert(MuonSach(root1, dsDS1, 20002, "IT002_1") == true);

  // 5. Lưu toàn bộ dữ liệu xuống đĩa
  cout << "3. Ghi du lieu Phien 1 xuong dia:\n";
  assert(GhiFileDocGia(root1, fileDG) == true);
  assert(ghiDauSach(dsDS1, fileDS) == true);
  cout << "   -> Ghi thanh cong " << fileDG << " va " << fileDS << "\n";

  // 6. GIẢI PHÓNG TOÀN BỘ BỘ NHỚ (MÔ PHỎNG TẮT CHƯƠNG TRÌNH)
  cout << "4. Thu hoi toan bo bo nho RAM (Mo phong dong ung dung)...\n";
  GiaiPhongCayDocGia(root1);
  giaiPhongDauSach(dsDS1);
  assert(KiemTraCayRong(root1) == true);
  assert(dauSachRong(dsDS1) == true);
  cout << "[PASS] Phien 1 ket thuc sach se khong ro ri bo nho.\n\n";

  // ##########################################################################
  // PHIÊN 2 (SESSION 2): PHỤC HỒI TỪ FILE VÀ KIỂM TRA TÍNH ĐỒNG BỘ
  // ##########################################################################
  cout << ">>> [PHIEN 2] Khoi dong moi ung dung, doc file va kiem tra dong bo...\n";

  TREE_DocGia root2;
  KhoiTaoCay(root2);
  DanhSachDauSach dsDS2;
  khoiTaoDSDS(dsDS2);

  // 1. Phục hồi dữ liệu từ 2 file
  assert(DocFileDocGia(root2, fileDG) == true);
  assert(docDauSach(dsDS2, fileDS) == true);
  cout << "1. Phuc hoi thanh cong tu file vao RAM.\n";

  // 2. Kiểm tra số lượng sau khi nạp
  assert(DemSoLuongDocGia(root2) == 2);
  assert(dsDS2.n == 2);
  cout << "2. Kiem tra so luong: 2 doc gia, 2 dau sach -> CHINH XAC.\n";

  // 3. Kiểm tra trạng thái sách trong DMS phục hồi đúng từ file
  DauSach *pDS = nullptr;
  NodeDMS *pCuon1 = nullptr;
  NodeDMS *pCuon2 = nullptr;

  // CS101_1 đã được mượn ở Phiên 1 -> trangThai phải bằng 1
  assert(timSachTheoMaSach(dsDS2, "CS101_1", pDS, pCuon1) == true);
  assert(pCuon1->data.trangThai == 1);
  cout << "3. Kiem tra trang thai cuon CS101_1: trangThai = 1 (Da muon) -> DONG BO HOAN HAO.\n";

  // IT002_1 đã được mượn ở Phiên 1 -> trangThai phải bằng 1
  assert(timSachTheoMaSach(dsDS2, "IT002_1", pDS, pCuon2) == true);
  assert(pCuon2->data.trangThai == 1);
  cout << "4. Kiem tra trang thai cuon IT002_1: trangThai = 1 (Da muon) -> DONG BO HOAN HAO.\n";

  // CS101_2 chưa ai mượn -> trangThai phải bằng 0 (Cho mượn)
  assert(timSachTheoMaSach(dsDS2, "CS101_2", pDS, pCuon1) == true);
  assert(pCuon1->data.trangThai == 0);
  cout << "5. Kiem tra trang thai cuon CS101_2: trangThai = 0 (Cho muon) -> CHINH XAC.\n";

  // 4. Kiểm tra bẫy lỗi ở Phiên 2: Độc giả Binh (20002) mượn CS101_1 (đang do An giữ) -> Phải từ chối!
  cout << "6. Kiem tra bao ve: Binh muon cuon CS101_1 dang luu hanh:\n";
  bool kqTrangThai = MuonSach(root2, dsDS2, 20002, "CS101_1");
  assert(kqTrangThai == false);
  cout << "[PASS] Tu choi muon sach dang duoc doc gia khac giu.\n\n";

  // 5. Độc giả An trả sách CS101_1 trong Phiên 2
  cout << "7. An tra cuon CS101_1 trong Phien 2:\n";
  assert(TraSach(root2, dsDS2, 20001, "CS101_1") == true);

  // Kiểm tra DMS lập tức chuyển về 0
  assert(timSachTheoMaSach(dsDS2, "CS101_1", pDS, pCuon1) == true);
  assert(pCuon1->data.trangThai == 0);
  cout << "   -> DMS cua CS101_1 tu dong cap nhat ve trangThai = 0.\n";

  // 6. Bây giờ Binh đã có thể mượn cuốn CS101_1 vừa được trả
  cout << "8. Binh muon lai cuon CS101_1 vua duoc An tra:\n";
  assert(MuonSach(root2, dsDS2, 20002, "CS101_1") == true);
  assert(pCuon1->data.trangThai == 1);
  cout << "[PASS] Giao dich muon sach dien ra thuan loi giua cac phien.\n\n";

  // 7. Giải phóng bộ nhớ Phiên 2
  GiaiPhongCayDocGia(root2);
  giaiPhongDauSach(dsDS2);
  cout << "9. Giai phong bo nho Phien 2 thanh cong.\n";

  cout << "=========================================================\n";
  cout << ">>> TINH BEN VUNG DU LIEU LIEN KET DAT PASS 100%! <<<<<\n";
  cout << "=========================================================\n";
  return 0;
}
