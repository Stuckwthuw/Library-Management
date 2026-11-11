#include "../Common/LuuTru.h"
#include "../DocGia.h"
#include "../Date.h"
#include "../MuonTra.h"
#include "../DauSach/DauSach_NoiBo.h"
#include <cstring>
#include <memory>

namespace {
struct Cuon { const NodeDMS *node; int dangMuon; };

// Kiểm tra tính hợp lệ và nhất quán của các lượt mượn trả
bool kiemTraDS(const NodeMuonTra *head, Cuon *a, int n, bool archive) {
    int dangGiu = 0;
    const char *isbnDangGiu[MAX_MUON_SACH] = {};
    const Date homNay = ngayHienTaiHeThong();
    for (const NodeMuonTra *p = head; p; p = p->pNext) {
        const MuonTra &m = p->data;
        if (!ngayHopLe(m.ngayMuon) || khoangCachNgay(m.ngayMuon, homNay) < 0 ||
            m.trangThai < 0 || m.trangThai > 2) return false;
        if (m.trangThai == 0) {
            if (archive || ++dangGiu > MAX_MUON_SACH ||
                m.ngayTra.ngay || m.ngayTra.thang || m.ngayTra.nam) return false;
            // Chỉ tối đa ba cuốn đang giữ: O(L * MAX_MUON_SACH), không quét
            // lại toàn bộ lịch sử cho từng giao dịch và không dùng container STL.
            for (int i = 0; i < dangGiu - 1; ++i)
                if (CungISBN(isbnDangGiu[i], m.maSach)) return false;
            isbnDangGiu[dangGiu - 1] = m.maSach;
        } else if (!ngayHopLe(m.ngayTra) || khoangCachNgay(m.ngayMuon, m.ngayTra) < 0 ||
                   khoangCachNgay(m.ngayTra, homNay) < 0) return false;
        int l = 0, r = n - 1, found = -1;
        while (l <= r) {
            int mid = l + (r - l) / 2;
            int cmp = std::strcmp(m.maSach, a[mid].node->data.maSach);
            if (!cmp) { found = mid; break; }
            if (cmp < 0) r = mid - 1; else l = mid + 1;
        }
        if (found < 0) return false;
        if (m.trangThai == 0 && ++a[found].dangMuon != 1) return false;
        if (m.trangThai == 2 && a[found].node->data.trangThai != 2) return false;
    }
    return true;
}
bool kiemTraCay(const NodeDocGia *r, Cuon *a, int n) {
    return !r || (kiemTraDS(r->data.dsMuonTra, a, n, false) &&
        kiemTraCay(r->pLeft, a, n) && kiemTraCay(r->pRight, a, n));
}
}

// Kiểm tra ràng buộc toàn vẹn giữa dữ liệu độc giả và dữ liệu đầu sách
bool KiemTraLienKetMuonTra(TREE_DocGia root, const DanhSachDauSach &ds,
                          std::string &loi) {
    try {
        int n = 0;
        for (int i = 0; i < ds.n; ++i)
            for (const NodeDMS *p = ds.nodes[i]->dms; p; p = p->pNext) ++n;
        std::unique_ptr<Cuon[]> a(new Cuon[n]);
        int idx = 0;
        for (int i = 0; i < ds.n; ++i)
            for (const NodeDMS *p = ds.nodes[i]->dms; p; p = p->pNext)
                a[idx++] = Cuon{p, 0};
        DauSachNoiBo::sapXep(a.get(), 0, n - 1, [](const Cuon &x, const Cuon &y) {
            return std::strcmp(x.node->data.maSach, y.node->data.maSach) < 0;
        });
        if (!kiemTraCay(root, a.get(), n) || !kiemTraDS(LayLichSuLuuTru(), a.get(), n, true)) {
            loi = "Lien ket muon tra hoac trang thai giua doc gia va dau sach khong nhat quan.";
            return false;
        }
        for (int i = 0; i < n; ++i) {
            if ((a[i].node->data.trangThai == 1) != (a[i].dangMuon == 1)) {
                loi = "Trang thai sach khong khop voi giao dich muon tra hien tai.";
                return false;
            }
        }
        loi.clear();
        return true;
    } catch (const std::exception &) {
        loi = "Khong the kiem tra lien ket giua hai bo du lieu.";
        return false;
    }
}
