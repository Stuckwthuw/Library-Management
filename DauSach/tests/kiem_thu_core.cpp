#include "../../DauSach.h"
#include "../../DanhMucSach.h"
#include "../../Date.h"
#include "../DauSach_NoiBo.h"
#include <cassert>
#include <cstring>
#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>

namespace {
DauSach *taoSach(const char *ISBN, const char *ten, const char *loai = "Tin hoc") {
    DauSach *d = new DauSach{};
    std::strcpy(d->ISBN, ISBN);
    std::strcpy(d->tenSach, ten);
    std::strcpy(d->tacGia, "Tac gia");
    std::strcpy(d->theLoai, loai);
    d->soTrang = 100;
    d->namXuatBan = 2026;
    return d;
}

void testDauSach() {
    std::unique_ptr<DanhSachDauSach> ds(new DanhSachDauSach{});
    khoiTaoDSDS(*ds);
    assert(dauSachRong(*ds) && !dauSachDay(*ds));
    assert(!timDauSachTheoISBN(*ds, "ZZ"));
    assert(!themDauSach(*ds, nullptr));
    DauSach *z = taoSach("AA", "Zulu", "Van hoc");
    DauSach *a = taoSach("ZZ", "Alpha");
    DauSach *m = taoSach("MM", "Middle", "Van hoc");
    assert(themDauSach(*ds, z) && themDauSach(*ds, a) && themDauSach(*ds, m));
    assert(ds->nodes[0] == a && ds->nodes[1] == m && ds->nodes[2] == z);
    assert(timDauSachTheoISBN(*ds, "AA") == z && timDauSachTheoISBN(*ds, "ZZ") == a);
    DauSach *dup = taoSach("AA", "Other");
    assert(!themDauSach(*ds, dup) && ds->n == 3);
    delete dup; dup = nullptr;

    DauSach *ketQua[3] = {};
    assert(timDauSachTheoTen(*ds, "ALP", ketQua, 3) == 1 && ketQua[0] == a);
    assert(timDauSachTheoTen(*ds, "missing", ketQua, 3) == 0);
    assert(timDauSachTheoTen(*ds, "ALP", ketQua, 1) == -1);
    assert(timDauSachTheoTen(*ds, "Middle", ds->nodes, MAX_DAUSACH) == -1);
    assert(layDauSachTheoTheLoai(*ds, ketQua, 3) == 3);
    assert(ketQua[0] == a && ketQua[1] == m && ketQua[2] == z);
    assert(layDauSachTheoTheLoai(*ds, ds->nodes, MAX_DAUSACH) == -1);
    assert(ds->nodes[0] == a && ds->nodes[1] == m && ds->nodes[2] == z);
#ifdef _WIN32
    assert(tenChuaTuKhoa("Cấu Trúc Dữ Liệu", "CẤU TRÚC"));
    assert(soSanhTen("Đại số", "Giải tích") < 0);
#endif

    assert(themBanSao(*a, 3, "Ke 1"));
    DauSach *out = nullptr; NodeDMS *cuon = nullptr;
    assert(timSachTheoMaSach(*ds, "ZZ_3", out, cuon) && out == a && cuon);
    assert(!timSachTheoMaSach(*ds, "ZZ_99", out, cuon) && !out && !cuon);
    assert(!timSachTheoMaSach(*ds, nullptr, out, cuon));

    // Chỉ mục phụ phải chuyển đúng khi xen kẽ truy vấn nhiều danh sách.
    std::unique_ptr<DanhSachDauSach> ds2(new DanhSachDauSach{});
    DauSach *khac = taoSach("AA", "Another");
    assert(themDauSach(*ds2, khac));
    assert(timDauSachTheoISBN(*ds2, "AA") == khac);
    assert(timDauSachTheoISBN(*ds, "AA") == z);
    giaiPhongDauSach(*ds2);
    giaiPhongDauSach(*ds);
    assert(ds->n == 0 && !ds->nodes[0]);

    // Chặn mảng đầy trước khi đụng tới phần tử hoặc nhận quyền sở hữu.
    ds->n = MAX_DAUSACH;
    DauSach *moi = taoSach("FULL", "Full");
    assert(dauSachDay(*ds) && !themDauSach(*ds, moi));
    delete moi; moi = nullptr;
    ds->n = 0;
}

void testDMS() {
    DauSach d{};
    std::strcpy(d.ISBN, "IT01");
    assert(themBanSao(d, 2, "Ke 1"));
    assert(demSoLuongDMS(d.dms) == 2);
    assert(capNhatTrangThaiSach(d.dms, 2));
    assert(themBanSao(d, 1, "Ke 2") && timKiemDMS(d.dms, "IT01_3"));
    assert(!capNhatTrangThaiSach(d.dms, 3) && d.dms->data.trangThai == 2);
    assert(!capNhatTrangThaiSach(nullptr, 0));
    NodeDMS *dup = taoNodeDMS("IT01_3", 0, "Ke 2");
    assert(!themCuoiDMS(d.dms, dup));
    delete dup; dup = nullptr;
    assert(!themBanSao(d, 0, "Ke 2") && !themBanSao(d, 1, ""));
    giaiPhongDMS(d.dms);
    assert(danhMucSachRong(d.dms));

    // Dãy có khoảng trống: dùng max STT, không dùng số lượng node + 1.
    assert(themCuoiDMS(d.dms, taoNodeDMS("IT01_5", 2, "Ke 1")));
    assert(themBanSao(d, 1, "Ke 1") && timKiemDMS(d.dms, "IT01_6"));
    giaiPhongDMS(d.dms);

    std::strcpy(d.ISBN, "12345678901234");
    assert(sinhMaSach(d.ISBN, 9999) == "12345678901234_9999");
    assert(!themBanSao(d, 10000, "Ke 1") && !d.dms);
    bool rejected = false;
    try { sinhMaSach(d.ISBN, 10000); } catch (const std::length_error &) { rejected = true; }
    assert(rejected);
}

void testTop10() {
    std::unique_ptr<DanhSachDauSach> ds(new DanhSachDauSach{});
    ThongKeSach top[10] = {};
    assert(layTop10(*ds, nullptr, top, 10) == 0);
    for (int i = 0; i < 12; ++i) {
        std::string ma = "B" + std::to_string(i);
        assert(themDauSach(*ds, taoSach(ma.c_str(), ma.c_str())));
    }
    // Fixture chỉ dùng struct chung; không cài/sửa module độc giả của Hiếu.
    NodeDocGia root{}, left{}, right{};
    root.pLeft = &left; root.pRight = &right;
    NodeMuonTra muon[4] = {};
    for (int i = 0; i < 3; ++i) {
        std::strcpy(muon[i].data.maSach, "B7_1");
        muon[i].data.trangThai = i; // Đang mượn, đã trả, làm mất đều tính một lượt.
    }
    std::strcpy(muon[3].data.maSach, "B3_1");
    root.data.dsMuonTra = &muon[0];
    left.data.dsMuonTra = &muon[1];
    right.data.dsMuonTra = &muon[2];
    muon[2].pNext = &muon[3];
    DauSach *dauTien = ds->nodes[0];
    assert(layTop10(*ds, &root, top, 10) == 10);
    assert(!std::strcmp(top[0].sach->ISBN, "B7") && top[0].soLuotMuon == 3);
    assert(!std::strcmp(top[1].sach->ISBN, "B3") && top[1].soLuotMuon == 1);
    assert(ds->nodes[0] == dauTien && root.data.dsMuonTra == &muon[0]);
    assert(layTop10(*ds, &root, top, 9) == -1);
    assert(layTop10(*ds, nullptr, top, 10) == 10 && top[0].soLuotMuon == 0);
    giaiPhongDauSach(*ds);
    assert(themDauSach(*ds, taoSach("S", "Solo")));
    assert(layTop10(*ds, nullptr, top, 10) == 1);
    giaiPhongDauSach(*ds);
}

void testNgay() {
    assert(!namNhuan(1900) && namNhuan(2000) && namNhuan(2024) && !namNhuan(2100));
    assert(!ngayHopLe(Date{29, 2, 2025}) && ngayHopLe(Date{29, 2, 2024}));
    assert(!ngayHopLe(Date{31, 4, 2026}) && !ngayHopLe(Date{1, 0, 2026}));
    assert(khoangCachNgay(Date{1, 10, 2026}, Date{8, 10, 2026}) == MAX_NGAY_MUON);
    assert(khoangCachNgay(Date{1, 10, 2026}, Date{9, 10, 2026}) > MAX_NGAY_MUON);
    Date d = congNgay(Date{11, 10, 2026}, -14);
    assert(d.ngay == 27 && d.thang == 9 && d.nam == 2026);
    d = congNgay(Date{1, 1, 2026}, -1);
    assert(d.ngay == 31 && d.thang == 12 && d.nam == 2025);
    assert(dinhDangNgay(Date{0, 0, 0}) == "--/--/----");
    bool rejected = false;
    try { congNgay(d, std::numeric_limits<int>::min()); }
    catch (const std::out_of_range &) { rejected = true; }
    assert(rejected);
    rejected = false;
    try { khoangCachNgay(Date{0, 0, 0}, d); }
    catch (const std::invalid_argument &) { rejected = true; }
    assert(rejected);

    // Kiểm tra liên tục 201 năm, bao gồm 1900/2000 và mọi biên tháng/năm.
    Date previous = {31, 12, 1900};
    for (int year = 1901; year <= 2101; ++year)
        for (int month = 1; month <= 12; ++month)
            for (int day = 1; day <= soNgayTrongThang(month, year); ++day) {
                Date current = {day, month, year};
                assert(khoangCachNgay(previous, current) == 1);
                assert(khoangCachNgay(current, previous) == -1);
                previous = current;
            }
}

void testSapXep() {
    int a[1000]; unsigned int seed = 7;
    for (int pass = 0; pass < 4; ++pass) {
        for (int i = 0; i < 1000; ++i) {
            seed = 1664525u * seed + 1013904223u;
            a[i] = pass == 0 ? i : pass == 1 ? 1000 - i : pass == 2 ? 5 : static_cast<int>(seed % 97);
        }
        DauSachNoiBo::sapXep(a, 0, 999, [](int x, int y) { return x < y; });
        for (int i = 1; i < 1000; ++i) assert(a[i - 1] <= a[i]);
    }
}
}

int main() {
    testDauSach(); testDMS(); testTop10(); testNgay(); testSapXep();
    std::cout << "PASS: DauSach, DMS, tim kiem, the loai, Top10, Date, QuickSort.\n";
}
