#include "../DocGia.h"
#include "../Date.h"
#include "../Common/FileUtils.h"
#include <cstring>
#include <stdexcept>

// ============================================================================
// PHÂN HỆ: QUẢN LÝ THẺ ĐỘC GIẢ (CÂY NHỊ PHÂN TÌM KIẾM - BST)
// FILE: DocGia_File.cpp - Đọc và ghi dữ liệu độc giả vào file
// PHỤ TRÁCH: LÊ VĂN HOÀNG HIẾU (N24DECE018)
// ============================================================================

namespace {
const int MAX_BAN_GHI = 1000000;
void can(bool ok) { if (!ok) throw std::runtime_error("File doc gia khong hop le"); }

bool luotHopLe(const MuonTra &m, bool luuTru) {
    if (!KiemTraDinhDangMaSach(m.maSach) || !ngayHopLe(m.ngayMuon) ||
        m.trangThai < 0 || m.trangThai > 2 || (luuTru && m.trangThai == 0)) return false;
    if (m.trangThai == 0)
        return m.ngayTra.ngay == 0 && m.ngayTra.thang == 0 && m.ngayTra.nam == 0;
    return ngayHopLe(m.ngayTra) && khoangCachNgay(m.ngayMuon, m.ngayTra) >= 0;
}

void ghiLuot(std::ostream &out, const NodeMuonTra *head) {
    int n = 0;
    for (const NodeMuonTra *p = head; p; p = p->pNext) ++n;
    out << n << '\n';
    for (const NodeMuonTra *p = head; p; p = p->pNext) {
        const MuonTra &m = p->data;
        can(luotHopLe(m, false));
        out << m.maSach << ' ' << m.ngayMuon.ngay << ' ' << m.ngayMuon.thang
            << ' ' << m.ngayMuon.nam << ' ' << m.ngayTra.ngay << ' '
            << m.ngayTra.thang << ' ' << m.ngayTra.nam << ' ' << m.trangThai << '\n';
    }
}

// Duyệt cây theo tiền thứ tự (NLR) để ghi file
void ghiCay(std::ostream &out, const NodeDocGia *root) {
    if (!root) return;
    const TheDocGia &d = root->data;
    can(d.maThe > 0 && d.maThe <= 999999 && (d.trangThai == 0 || d.trangThai == 1) &&
        KiemTraChuCaiVaKhoangTrang(d.ho) && KiemTraChuCaiVaKhoangTrang(d.ten) &&
        (!std::strcmp(d.phai, "Nam") || !std::strcmp(d.phai, "Nu")));
    out << d.maThe << '\n' << d.ho << '\n' << d.ten << '\n'
        << d.phai << '\n' << d.trangThai << '\n';
    ghiLuot(out, d.dsMuonTra);
    ghiCay(out, root->pLeft);
    ghiCay(out, root->pRight);
}

void docLuot(std::istream &in, NodeMuonTra *&head, bool luuTru) {
    int n = 0;
    can(docSo(in, n) && n >= 0 && n <= MAX_BAN_GHI);
    NodeMuonTra **tail = &head;
    int dangMuon = 0;
    for (int i = 0; i < n; ++i) {
        std::string line, ma;
        can(bool(std::getline(in, line)));
        std::istringstream record(line);
        MuonTra m{};
        can(bool(record >> ma >> m.ngayMuon.ngay >> m.ngayMuon.thang >> m.ngayMuon.nam
            >> m.ngayTra.ngay >> m.ngayTra.thang >> m.ngayTra.nam >> m.trangThai));
        record >> std::ws;
        can(record.eof() && ma.size() < sizeof(m.maSach));
        std::strcpy(m.maSach, ma.c_str());
        can(luotHopLe(m, luuTru));
        if (m.trangThai == 0) {
            can(++dangMuon <= MAX_MUON_SACH);
            for (NodeMuonTra *p = head; p; p = p->pNext)
                can(p->data.trangThai != 0 || std::strcmp(p->data.maSach, m.maSach));
        }
        *tail = new NodeMuonTra{m, nullptr};
        tail = &(*tail)->pNext;
    }
}

void docTheCu(const std::string &line, TheDocGia &d) {
    std::istringstream row(line);
    std::string fields, part;
    for (int i = 0; i < 5; ++i) {
        can(bool(std::getline(row, part, ',')));
        fields += part + '\n';
    }
    can(row.eof());
    std::istringstream in(fields);
    can(docSo(in, d.maThe) && docChuoi(in, d.ho) && docChuoi(in, d.ten) &&
        docChuoi(in, d.phai) && docSo(in, d.trangThai));
}
}

// Ghi toàn bộ dữ liệu độc giả vào file
bool GhiFileDocGia(TREE_DocGia root, const char *path) {
    if (!path || !*path) return false;
    try {
        std::ostringstream out;
        out << "DOCGIA_V3\n" << DemSoLuongDocGia(root) << '\n';
        ghiCay(out, root);
        for (const NodeMuonTra *p = LayLichSuLuuTru(); p; p = p->pNext)
            can(luotHopLe(p->data, true));
        ghiLuot(out, LayLichSuLuuTru());
        return bool(out) && ghiDuLieuMotDong(out.str(), path);
    } catch (const std::exception &) { return false; }
}

// Đọc toàn bộ dữ liệu độc giả từ file
bool DocFileDocGia(TREE_DocGia &root, const char *path) {
    if (!path || !*path) return false;
    TREE_DocGia tam = nullptr;
    NodeMuonTra *archive = nullptr;
    TheDocGia d{};
    try {
        std::ifstream file(path, std::ios::binary);
        can(bool(file));
        std::ostringstream bytes;
        bytes << file.rdbuf();
        can(!file.bad());
        std::string text = bytes.str(), fields;
        const bool versioned = text.compare(0, 9, "DOCGIA_V3") == 0;
        if (versioned) can(docDuLieuFile(path, fields));
        else fields = text;
        std::istringstream in(fields);
        int n = 0;
        std::string line;
        if (versioned) {
            can(bool(std::getline(in, line)) && line == "DOCGIA_V3");
            can(docSo(in, n) && n >= 0 && n <= 999999);
        }
        int count = 0;
        while (true) {
            if (versioned && count == n) break;
            if (!versioned) {
                if (!std::getline(in, line)) { can(in.eof()); break; }
                if (!line.empty() && line.back() == '\r') line.pop_back();
                can(!line.empty());
                docTheCu(line, d);
            } else {
                can(docSo(in, d.maThe) && docChuoi(in, d.ho) && docChuoi(in, d.ten) &&
                    docChuoi(in, d.phai) && docSo(in, d.trangThai));
            }
            can(++count <= 999999);
            docLuot(in, d.dsMuonTra, false);
            can(ThemDocGia(tam, d));
            d.dsMuonTra = nullptr;
        }
        if (versioned) docLuot(in, archive, true);
        in >> std::ws;
        can(in.eof());
        GiaiPhongCayDocGia(root);
        root = tam;
        tam = nullptr;
        ThayLichSuLuuTru(archive);
        return true;
    } catch (const std::exception &) {
        GiaiPhongDSMuonTra(d.dsMuonTra);
        GiaiPhongDSMuonTra(archive);
        GiaiPhongCayDocGia(tam);
        return false;
    }
}
