#include "../DauSach.h"
#include "../DanhMucSach.h"

// ============================================================================
// PHÂN HỆ: QUẢN LÝ ĐẦU SÁCH (MẢNG CON TRỎ)
// FILE: DauSach_Core.cpp — Khởi tạo và giải phóng mảng con trỏ đầu sách
// PHỤ TRÁCH: LÊ ANH ĐỨC (N24DECE012)
// ============================================================================

// Khởi tạo danh sách đầu sách
void khoiTaoDSDS(DanhSachDauSach &ds) {
    ds.n = 0;
    for (int i = 0; i < MAX_DAUSACH; ++i) ds.nodes[i] = nullptr;
    voHieuChiMucISBN();
}

bool dauSachRong(const DanhSachDauSach &ds) { return ds.n == 0; }
bool dauSachDay(const DanhSachDauSach &ds) { return ds.n >= MAX_DAUSACH; }

// Giải phóng toàn bộ mảng con trỏ đầu sách và các danh mục sách con
void giaiPhongDauSach(DanhSachDauSach &ds) {
    for (int i = 0; i < ds.n; ++i) {
        if (ds.nodes[i]) {
            giaiPhongDMS(ds.nodes[i]->dms);
            delete ds.nodes[i];
            ds.nodes[i] = nullptr;
        }
    }
    ds.n = 0;
    voHieuChiMucISBN();
}
