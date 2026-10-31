#include "CauTruc.h"
#include "DocGia.h"
#include "MuonTra.h"
#include "DauSach.h"
#include "DanhMucSach.h"
#include "Date.h"
#include <cassert>
#include <cstring>
#include <iostream>

using namespace std;

// ============================================================================
// CHƯƠNG TRÌNH KIỂM THỬ TÍCH HỢP GIAI ĐOẠN 3 (INTEGRATION TEST)
// Kiểm tra sự phối hợp hoàn chỉnh giữa Module của Hiếu và Module của Đức:
//   - Hiếu: Cây BST Độc giả, DSLK Mượn Trả
//   - Đức: Mảng con trỏ Đầu Sách, DSLK DMS, Module Ngày tháng Date, Top 10
// ============================================================================

DauSach *TaoDauSachTest(const char *isbn, const char *ten, const char *theLoai) {
  DauSach *ds = new DauSach{};
  strcpy(ds->ISBN, isbn);
  strcpy(ds->tenSach, ten);
  strcpy(ds->tacGia, "Tac gia");
  strcpy(ds->theLoai, theLoai);
  ds->soTrang = 350;
  ds->namXuatBan = 2026;
  ds->dms = nullptr;
  return ds;
}

int main() {
  cout << "=========================================================\n";
  cout << "=== KIEM THU TICH HOP GIAI DOAN 3: GHEP NOI HE THONG ====\n";
  cout << "=========================================================\n\n";

  // --------------------------------------------------------------------------
  // BƯỚC 1: KHỞI TẠO CẤU TRÚC DỮ LIỆU
  // --------------------------------------------------------------------------
  TREE_DocGia root;
  KhoiTaoCay(root);

  DanhSachDauSach dsDS;
  khoiTaoDSDS(dsDS);

  // --------------------------------------------------------------------------
  // BƯỚC 2: NẠP DỮ LIỆU ĐẦU SÁCH & BẢN SAO DMS (MODULE ĐỨC)
  // --------------------------------------------------------------------------
  cout << "--- 1. Nap Dau Sach & Danh Muc Sach (Module Duc) ---\n";
  DauSach *sach1 = TaoDauSachTest("CS101", "Cau Truc Du Lieu & Giai Thuat", "CNTT");
  DauSach *sach2 = TaoDauSachTest("IT002", "Lap Trinh Huong Doi Tuong C++", "CNTT");
  themDauSach(dsDS, sach1);
  themDauSach(dsDS, sach2);

  // Thêm bản sao: CS101_1, CS101_2, CS101_3
  themBanSao(*sach1, 3, "Ke A1 - Ngan 1");
  // Thêm bản sao: IT002_1, IT002_2
  themBanSao(*sach2, 2, "Ke A2 - Ngan 2");

  assert(dsDS.n == 2);
  cout << "[PASS] Nap thanh cong 2 dau sach va 5 ban sao DMS.\n\n";

  // --------------------------------------------------------------------------
  // BƯỚC 3: NẠP ĐỘC GIẢ (MODULE HIẾU)
  // --------------------------------------------------------------------------
  cout << "--- 2. Nap The Doc Gia (Module Hieu) ---\n";
  TheDocGia dg1;
  dg1.maThe = 10001;
  strcpy(dg1.ho, "Nguyen Van");
  strcpy(dg1.ten, "An");
  strcpy(dg1.phai, "Nam");
  dg1.trangThai = 1; // Hoạt động
  dg1.dsMuonTra = nullptr;
  ThemDocGia(root, dg1);

  TheDocGia dg2;
  dg2.maThe = 10002;
  strcpy(dg2.ho, "Tran Thi");
  strcpy(dg2.ten, "Binh");
  strcpy(dg2.phai, "Nu");
  dg2.trangThai = 0; // Thẻ bị khóa
  dg2.dsMuonTra = nullptr;
  ThemDocGia(root, dg2);

  assert(DemSoLuongDocGia(root) == 2);
  cout << "[PASS] Nap thanh cong 2 doc gia vao cay BST.\n\n";

  // --------------------------------------------------------------------------
  // BƯỚC 4: TEST NGHIỆP VỤ MƯỢN SÁCH & BẪY LỖI
  // --------------------------------------------------------------------------
  cout << "--- 3. Kiem thu Nghiep vu Muon Sach ---\n";

  // Case 4.1: Độc giả thẻ bị khóa mượn sách → Bị từ chối
  cout << "-> Test 3.1: Doc gia bi khoa the (Ma: 10002) muon sach:\n";
  bool kqKhoa = MuonSach(root, dsDS, 10002, "CS101_1");
  assert(kqKhoa == false);
  cout << "[PASS] Tu choi dung quy tac khi the bi khoa.\n\n";

  // Case 4.2: Độc giả 10001 mượn sách CS101_1 hợp lệ
  cout << "-> Test 3.2: Doc gia 10001 muon cuon CS101_1:\n";
  bool kqMuon1 = MuonSach(root, dsDS, 10001, "CS101_1");
  assert(kqMuon1 == true);

  // Kiểm tra trạng thái sách trong DMS của Đức tự động chuyển sang 1 (Đã mượn)
  DauSach *pDSCheck = nullptr;
  NodeDMS *pCuonCheck = nullptr;
  assert(timSachTheoMaSach(dsDS, "CS101_1", pDSCheck, pCuonCheck) == true);
  assert(pCuonCheck->data.trangThai == 1);
  cout << "[PASS] Muon thanh cong va DMS tu dong chuyen sang trangThai = 1.\n\n";

  // Case 4.3: Độc giả mượn trùng cuốn sách đang giữ → Bị từ chối
  cout << "-> Test 3.3: Doc gia 10001 muon trung cuon CS101_1:\n";
  bool kqTrung = MuonSach(root, dsDS, 10001, "CS101_1");
  assert(kqTrung == false);
  cout << "[PASS] Tu choi muon trung thanh cong.\n\n";

  // Case 4.4: Mượn thêm cuốn 2 và cuốn 3
  cout << "-> Test 3.4: Doc gia 10001 muon tiep CS101_2 va IT002_1:\n";
  assert(MuonSach(root, dsDS, 10001, "CS101_2") == true);
  assert(MuonSach(root, dsDS, 10001, "IT002_1") == true);
  cout << "[PASS] Muon thanh cong du 3 cuon sach.\n\n";

  // Case 4.5: Mượn cuốn thứ 4 → Bị từ chối vì MAX_MUON_SACH = 3
  cout << "-> Test 3.5: Doc gia 10001 muon cuon thu 4 (IT002_2):\n";
  bool kqMax = MuonSach(root, dsDS, 10001, "IT002_2");
  assert(kqMax == false);
  cout << "[PASS] Tu choi vi da muon toi da 3 cuon.\n\n";

  // --------------------------------------------------------------------------
  // BƯỚC 5: TEST LIỆT KÊ SÁCH ĐANG MƯỢN (HIỂN THỊ TÊN TỪ MODULE ĐỨC)
  // --------------------------------------------------------------------------
  cout << "--- 4. Kiem thu Liet ke sach dang muon (kem Ten Sach tu DMS) ---\n";
  LietKeSachDangMuon(root, dsDS, 10001);
  cout << "[PASS] Bang liet ke hien thi day du va chinh xac Ten Sach.\n\n";

  // --------------------------------------------------------------------------
  // BƯỚC 6: TEST TRẢ SÁCH
  // --------------------------------------------------------------------------
  cout << "--- 5. Kiem thu Nghiep vu Tra Sach ---\n";
  cout << "-> Doc gia 10001 tra cuon CS101_1:\n";
  assert(TraSach(root, dsDS, 10001, "CS101_1") == true);

  // Kiểm tra DMS tự động trả về trangThai = 0 (Cho mượn)
  assert(timSachTheoMaSach(dsDS, "CS101_1", pDSCheck, pCuonCheck) == true);
  assert(pCuonCheck->data.trangThai == 0);
  cout << "[PASS] Tra sach thanh cong va DMS tu dong chuyen ve trangThai = 0.\n\n";

  // --------------------------------------------------------------------------
  // BƯỚC 7: TEST BÁO MẤT SÁCH
  // --------------------------------------------------------------------------
  cout << "--- 6. Kiem thu Nghiep vu Bao Mat Sach ---\n";
  cout << "-> Doc gia 10001 bao mat cuon CS101_2:\n";
  assert(BaoMatSach(root, dsDS, 10001, "CS101_2") == true);

  // Kiểm tra DMS tự động chuyển sang trangThai = 2 (Thanh lý)
  assert(timSachTheoMaSach(dsDS, "CS101_2", pDSCheck, pCuonCheck) == true);
  assert(pCuonCheck->data.trangThai == 2);
  cout << "[PASS] Bao mat sach thanh cong va DMS tu dong chuyen sang trangThai = 2.\n\n";

  // --------------------------------------------------------------------------
  // BƯỚC 8: TEST THỐNG KÊ TOP 10 CỦA ĐỨC DỰA TRÊN LỊCH SỬ MƯỢN TRẢ CỦA HIẾU
  // --------------------------------------------------------------------------
  cout << "--- 7. Kiem thu Thong ke Top 10 (Module Duc doc du lieu tu Cay Hieu) ---\n";
  ThongKeSach top10[10];
  int soLuongTop = layTop10(dsDS, root, top10, 10);
  assert(soLuongTop == 2);
  // Đầu sách CS101 có 2 lượt mượn (CS101_1, CS101_2)
  assert(strcmp(top10[0].sach->ISBN, "CS101") == 0);
  assert(top10[0].soLuotMuon == 2);
  // Đầu sách IT002 có 1 lượt mượn (IT002_1)
  assert(strcmp(top10[1].sach->ISBN, "IT002") == 0);
  assert(top10[1].soLuotMuon == 1);

  cout << "Ket qua Top 10 sach muon nhieu nhat:\n";
  for (int i = 0; i < soLuongTop; ++i) {
    cout << "  Top " << (i + 1) << ": [" << top10[i].sach->ISBN << "] "
         << top10[i].sach->tenSach << " - " << top10[i].soLuotMuon << " luot muon.\n";
  }
  cout << "[PASS] Module Top 10 doc dung lich su va tinh toan chinh xac!\n\n";

  // --------------------------------------------------------------------------
  // BƯỚC 9: GIẢI PHÓNG BỘ NHỚ CẢ HAI PHÂN HỆ
  // --------------------------------------------------------------------------
  cout << "--- 8. Thu hoi toan bo bo nho cap phat (Chong Memory Leak) ---\n";
  GiaiPhongCayDocGia(root);
  assert(KiemTraCayRong(root) == true);

  giaiPhongDauSach(dsDS);
  assert(dauSachRong(dsDS) == true);
  cout << "[PASS] Toan bo bo nho cay BST va mang con tro da duoc thu hoi an toan.\n\n";

  cout << "=========================================================\n";
  cout << ">>> TAT CA 8 HANG MUC GHEP NOI DA THANH CONG HOAN HAO! <<<\n";
  cout << "=========================================================\n";
  return 0;
}

