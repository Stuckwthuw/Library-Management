#include "../DocGia.h"
#include "../DauSach.h"
#include "../DanhMucSach.h"
#include "../MuonTra.h"
#include "../Date.h"
#include "../Common/LuuTru.h"
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <windows.h>

namespace {
std::string bytes(const char *path) {
    std::ifstream in(path, std::ios::binary);
    std::ostringstream out; out << in.rdbuf(); return out.str();
}
void write(const char *path, const std::string &s) {
    std::ofstream out(path, std::ios::binary); out << s; out.close(); assert(out);
}
TheDocGia reader(int id) {
    TheDocGia d{}; d.maThe = id; d.trangThai = 1;
    std::strcpy(d.ho, "Nguyen Van"); std::strcpy(d.ten, "An"); std::strcpy(d.phai, "Nam");
    return d;
}
DauSach *book(DanhSachDauSach &ds, const char *isbn = "AUDIT") {
    DauSach *d = new DauSach{};
    std::strcpy(d->ISBN, isbn); std::strcpy(d->tenSach, "Test Book");
    std::strcpy(d->tacGia, "Test Author"); std::strcpy(d->theLoai, "Test");
    d->soTrang = 100; d->namXuatBan = 2026;
    assert(themBanSao(*d, 4, "Ke A1")); assert(themDauSach(ds, d)); return d;
}
int countHistory(const NodeMuonTra *p) {
    int n = 0; for (; p; p = p->pNext) ++n; return n;
}
void testNhap() {
    char s[50] = u8"  đẶNG   VĂN   ánh ";
    assert(DinhDangHoTen(s) && !std::strcmp(s, u8"Đặng Văn Ánh"));
    assert(ChuaChuoiConKhongPhanBietHoaThuong(u8"Đặng Văn Ánh", u8"đẶNG"));
    assert(!KiemTraChuCaiVaKhoangTrang(u8"An 😀"));
    assert(!KiemTraChuCaiVaKhoangTrang("An123"));
    assert(!KiemTraChuCaiVaKhoangTrang("\xFF"));
    assert(!KiemTraDinhDangMaSach("A_9999999999999999999"));

}

void testBST() {
    TREE_DocGia root = nullptr;
    assert(ThemDocGia(root, reader(20000)) && ThemDocGia(root, reader(10000)) && ThemDocGia(root, reader(30000)));
    assert(!ThemDocGia(root, reader(20000)));
    TheDocGia invalid = reader(-1); assert(!ThemDocGia(root, invalid));
    assert(!HieuChinhDocGia(root, 20000, "Nguyen", "An", "Bad", 1));
    assert(HieuChinhDocGia(root, 20000, u8"đẶNG", u8"ánh", "Nu", 0));
    NodeMuonTra *h = new NodeMuonTra{}; h->data.trangThai = 1;
    root->pRight->data.dsMuonTra = h;
    assert(XoaDocGia(root, 20000) && root->data.maThe == 30000 && root->data.dsMuonTra == h);
    assert(XoaDocGia(root, 10000) && XoaDocGia(root, 30000) && !root);
    assert(LayLichSuLuuTru() == h);
    GiaiPhongLichSuLuuTru();
    assert(ThemDocGia(root, reader(20000)) && ThemDocGia(root, reader(10000)));
    assert(XoaDocGia(root, 20000) && root->data.maThe == 10000);
    GiaiPhongCayDocGia(root);
    // Bộ sinh kiểm tra mã đang dùng; 1000 lần cấp không trùng trong cây.
    for (int i = 0; i < 1000; ++i) {
        int id = SinhMaTheTuDong(root);
        assert(id >= 10000 && id <= 99999 && !TimKiemDocGia(root, id));
        assert(ThemDocGia(root, reader(id)));
    }
    assert(DemSoLuongDocGia(root) == 1000);
    GiaiPhongCayDocGia(root);
}

void testTenVaSapXep() {
    assert(KiemTraTenMotTu(u8"  hiếu  "));
    for (const char *s : {"", "   ", "Hoang Hieu", "Anh\tThu", "An2", "An!"})
        assert(!KiemTraTenMotTu(s));
    TREE_DocGia root = nullptr;
    TheDocGia d = reader(50000); std::strcpy(d.ten, "Hoang Hieu");
    assert(!ThemDocGia(root, d) && !root);
    assert(ThemDocGia(root, reader(50000)));
    assert(!HieuChinhDocGia(root, 50000, "Le Van", "Hoang Hieu", "Nam", 1));
    assert(!std::strcmp(root->data.ten, "An"));
    TheDocGia a = reader(3), b = reader(2), c = reader(1), e = reader(4);
    std::strcpy(a.ten, "Khoa"); std::strcpy(b.ten, "Hieu");
    std::strcpy(e.ho, "Tran");
    TheDocGia *arr[] = {&a, &e, &b, &c};
    QuickSortDocGia(arr, 0, 3);
    assert(arr[0] == &c && arr[1] == &e && arr[2] == &b && arr[3] == &a);
    e = c; e.maThe = 9;
    assert(SoSanhTenHo(&c, &e) < 0);
    assert(CungISBN("A_1", "A_2") && !CungISBN("A_1", "AB_2"));

    std::string preview;
    write("old-name.txt", "DOCGIA_V3 | 1 | 60000 | Le Van | Hoang Hieu | Nam | 1 | 0 | 0\n");
    assert(DocFileDocGia(root, "old-name.txt", &preview));
    assert(!std::strcmp(root->data.ho, "Le Van Hoang") && !std::strcmp(root->data.ten, "Hieu"));
    assert(preview.find("Le Van Hoang | Hieu") != std::string::npos);
    NodeDocGia *old = root;
    write("overflow-name.txt", std::string("60000,") + std::string(48, 'A') + ",Hoang Hieu,Nam,1\n0\n");
    assert(!DocFileDocGia(root, "overflow-name.txt") && root == old);
    const Date today = LayNgayHienTai();
    std::ostringstream bad;
    bad << "60000,Le Van,Hieu,Nam,1\n2\n";
    for (int i = 1; i <= 2; ++i)
        bad << "AUDIT_" << i << ' ' << today.ngay << ' ' << today.thang << ' ' << today.nam << " 0 0 0 0\n";
    write("duplicate-isbn.txt", bad.str());
    assert(!DocFileDocGia(root, "duplicate-isbn.txt") && root == old);
    GiaiPhongCayDocGia(root);
}

void testGiaoDich() {
    TREE_DocGia root = nullptr; DanhSachDauSach ds{}; DauSach *d = book(ds);
    assert(ThemDocGia(root, reader(50000)));
    root->data.trangThai = 0; assert(!MuonSach(root, ds, 50000, "AUDIT_1"));
    root->data.trangThai = 1;
    assert(MuonSach(root, ds, 50000, "AUDIT_1"));
    assert(!MuonSach(root, ds, 50000, "AUDIT_1") && !XoaDocGia(root, 50000));
    root->data.dsMuonTra->data.ngayMuon = congNgay(LayNgayHienTai(), -7);
    assert(!KiemTraCoSachQuaHan(root->data.dsMuonTra));
    root->data.dsMuonTra->data.ngayMuon = congNgay(LayNgayHienTai(), -8);
    assert(KiemTraCoSachQuaHan(root->data.dsMuonTra) && !MuonSach(root, ds, 50000, "AUDIT_2"));
    root->data.dsMuonTra->data.ngayMuon = LayNgayHienTai();
    assert(!MuonSach(root, ds, 50000, "AUDIT_2"));
    assert(DemSoSachDangMuon(root->data.dsMuonTra) == 1);
    assert(timKiemDMS(d->dms, "AUDIT_2")->data.trangThai == 0);
    DauSach *other = book(ds, "OTHER"); book(ds, "THIRD"); book(ds, "FOURTH");
    assert(MuonSach(root, ds, 50000, "OTHER_1") && MuonSach(root, ds, 50000, "THIRD_1"));
    assert(!MuonSach(root, ds, 50000, "FOURTH_1"));
    NodeDMS *cuon = timKiemDMS(d->dms, "AUDIT_1");
    cuon->data.trangThai = 0;
    assert(!TraSach(root, ds, 50000, "AUDIT_1") && DemSoSachDangMuon(root->data.dsMuonTra) == 3);
    cuon->data.trangThai = 1;
    assert(TraSach(root, ds, 50000, "AUDIT_1"));
    const MuonTra *dangMuon[MAX_MUON_SACH];
    assert(LaySachDangMuon(root, 50000, dangMuon, MAX_MUON_SACH) == 2);
    assert(BaoMatSach(root, ds, 50000, "OTHER_1") && timKiemDMS(other->dms, "OTHER_1")->data.trangThai == 2);
    assert(TraSach(root, ds, 50000, "THIRD_1"));
    assert(MuonSach(root, ds, 50000, "AUDIT_2"));
    assert(TraSach(root, ds, 50000, "AUDIT_2"));
    ThongKeSach top[10]; assert(layTop10(ds, root, top, 10, LayLichSuLuuTru()) == 4 && top[0].soLuotMuon == 2);
    assert(XoaDocGia(root, 50000));
    assert(layTop10(ds, root, top, 10, LayLichSuLuuTru()) == 4 && top[0].soLuotMuon == 2);
    std::string loi;
    assert(LuuDuLieu(root, ds, "readers.txt", "books.txt", loi));
    const NodeMuonTra *archive = LayLichSuLuuTru();
    write("archive-bad.txt", "BROKEN\n");
    assert(!DocFileDocGia(root, "archive-bad.txt") && LayLichSuLuuTru() == archive);
    GiaiPhongLichSuLuuTru(); giaiPhongDauSach(ds);
    assert(NapDuLieu(root, ds, "readers.txt", "books.txt", loi));
    assert(!root && countHistory(LayLichSuLuuTru()) == 4);
    assert(layTop10(ds, root, top, 10, LayLichSuLuuTru()) == 4 && top[0].soLuotMuon == 2);
    giaiPhongDauSach(ds); GiaiPhongLichSuLuuTru();
}

void testFileLoi() {
    TREE_DocGia root = nullptr; assert(ThemDocGia(root, reader(50000)));
    write("legacy.txt", "20000,Nguyen,An,Nam,1\n0\n10000,Tran,Binh,Nu,0\n0\n30000,Le,Chi,Nam,1\n0\n");
    assert(DocFileDocGia(root, "legacy.txt") && root->data.maThe == 20000);
    assert(root->pLeft->data.maThe == 10000 && root->pRight->data.maThe == 30000);
    assert(GhiFileDocGia(root,"roundtrip.txt"));
    assert(bytes("roundtrip.txt").find("DOCGIA_V3 | 3") == 0);
    assert(DocFileDocGia(root,"roundtrip.txt") && root->pLeft->data.maThe == 10000);
    const std::string cases[] = {
        "NOT_A_VALID_READER_FILE\n", "1,Nguyen,An,Nam\n", "1,Nguyen,An,Nam,1,EXTRA\n0\n",
        "1,Nguyen,An,Nam,1\n1\nAUDIT_1 31 2 2026 0 0 0 0\n",
        "1,Nguyen,An,Nam,1\n-1\n", "1,Nguyen,An,Nam,9\n0\n",
        "1,Nguyen,An,Bad,1\n0\n", "1,Nguyen,An,Nam,1\n0\n1,Tran,Binh,Nam,1\n0\n",
        "1,Nguyen,An,Nam,1\n1\nAUDIT_1 1 1 2026 0 0 0 1\n",
        "DOCGIA_V3 | 0 | 1 | AUDIT_1 1 1 2026 0 0 0 0\n",
        "DOCGIA_V3 | 0 | 0 | EXTRA\n"
    };
    NodeDocGia *old = root;
    for (const std::string &bad : cases) {
        write("bad.txt", bad);
        assert(!DocFileDocGia(root, "bad.txt") && root == old && DemSoLuongDocGia(root) == 3);
    }
    // Ghi thất bại do không thể tạo file tạm không được phá bản dữ liệu cũ.
    std::string before = bytes("roundtrip.txt");
    CreateDirectoryA("roundtrip.txt.tmp", nullptr);
    assert(!GhiFileDocGia(root, "roundtrip.txt") && bytes("roundtrip.txt") == before);
    RemoveDirectoryA("roundtrip.txt.tmp");
    GiaiPhongCayDocGia(root);
}

void testPhucHoi() {
    TREE_DocGia root = nullptr; DanhSachDauSach ds{}; book(ds);
    assert(ThemDocGia(root, reader(50000)));
    std::string loi;
    assert(LuuDuLieu(root, ds, "tx-readers.txt", "tx-books.txt", loi));
    std::string oldBooks = bytes("tx-books.txt"), oldReaders = bytes("tx-readers.txt");
    assert(MuonSach(root, ds, 50000, "AUDIT_1"));
    // Khóa file thứ hai chống thay thế: mô phỏng dừng giữa hai lần công bố file.
    HANDLE lock = CreateFileA("tx-books.txt", GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
    assert(lock != INVALID_HANDLE_VALUE);
    assert(!LuuDuLieu(root, ds, "tx-readers.txt", "tx-books.txt", loi));
    assert(bytes("tx-books.txt") == oldBooks && bytes("tx-readers.txt") != oldReaders);
    assert(GetFileAttributesA("tx-readers.txt.pending") != INVALID_FILE_ATTRIBUTES);
    CloseHandle(lock);
    GiaiPhongCayDocGia(root); giaiPhongDauSach(ds);
    assert(NapDuLieu(root, ds, "tx-readers.txt", "tx-books.txt", loi));
    assert(DemSoSachDangMuon(root->data.dsMuonTra) == 1);
    assert(timKiemDMS(ds.nodes[0]->dms, "AUDIT_1")->data.trangThai == 1);
    assert(GetFileAttributesA("tx-readers.txt.pending") == INVALID_FILE_ATTRIBUTES);
    assert(PhucHoiGiaoDich("tx-readers.txt", "tx-books.txt", loi));
    // Nếu cặp file nạp không hợp lệ, dữ liệu trên RAM phải được giữ nguyên.
    NodeDocGia *oldRoot = root;
    DauSach *oldBook = ds.nodes[0];
    write("load-readers.txt", "60000,Tran,An,Nam,1\n0\n");
    write("load-books.txt", "BROKEN\n");
    assert(!NapDuLieu(root, ds, "load-readers.txt", "load-books.txt", loi));
    assert(root == oldRoot && ds.nodes[0] == oldBook);
    // Nhật ký hỏng phải được giữ để xử lý, không ghi đè file dữ liệu đang có.
    oldReaders = bytes("tx-readers.txt");
    write("tx-readers.txt.pending", "LIBRARY_TX1\n100 100 0 0\ncut");
    assert(!PhucHoiGiaoDich("tx-readers.txt", "tx-books.txt", loi));
    assert(bytes("tx-readers.txt") == oldReaders);
    std::remove("tx-readers.txt.pending");
    // Không có hai file là khởi động mới; thiếu chỉ một file là lỗi.
    TREE_DocGia empty = nullptr; DanhSachDauSach emptyDS{};
    assert(NapDuLieu(empty, emptyDS, "new-readers.txt", "new-books.txt", loi));
    write("new-readers.txt", "");
    assert(!NapDuLieu(empty, emptyDS, "new-readers.txt", "new-books.txt", loi));
    // DMS báo đang mượn nhưng không có giao dịch tương ứng: từ chối lưu.
    timKiemDMS(ds.nodes[0]->dms, "AUDIT_2")->data.trangThai = 1;
    assert(!LuuDuLieu(root, ds, "tx-readers.txt", "tx-books.txt", loi));
    assert(bytes("tx-readers.txt") == oldReaders);
    GiaiPhongCayDocGia(root); giaiPhongDauSach(ds); GiaiPhongLichSuLuuTru();
}
}

int main() {
    testNhap(); testBST(); testTenVaSapXep(); testGiaoDich(); testFileLoi(); testPhucHoi();
    std::cout << "PASS: tich hop, Unicode, BST, muon-tra, luu tru, phuc hoi giao dich.\n";
}
