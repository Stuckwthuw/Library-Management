#include "../DauSach.h"
#include "../DanhMucSach.h"

// Chỉ khởi tạo danh sách mới; danh sách đang có dữ liệu phải được giải phóng trước.
void khoiTaoDSDS(DanhSachDauSach &ds) {
    ds.n = 0;
    for (int i = 0; i < MAX_DAUSACH; ++i) ds.nodes[i] = nullptr;
    voHieuChiMucISBN();
}

bool dauSachRong(const DanhSachDauSach &ds) { return ds.n == 0; }
bool dauSachDay(const DanhSachDauSach &ds) { return ds.n >= MAX_DAUSACH; }

// Thu hồi DMS trước đầu sách để không mất con trỏ tới các node con; O(N + tổng cuốn).
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
