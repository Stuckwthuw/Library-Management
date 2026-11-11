#include "../Common/LuuTru.h"
#include "../DocGia.h"
#include "../DauSach.h"
#include "../Common/FileUtils.h"
#include <cstdint>
#include <memory>

namespace {
const std::size_t MAX_ANH = 256u * 1024u * 1024u;
int tonTai(const char *path) {
    DWORD value = GetFileAttributesA(path);
    if (value != INVALID_FILE_ATTRIBUTES) return (value & FILE_ATTRIBUTE_DIRECTORY) ? -1 : 1;
    DWORD error = GetLastError();
    return error == ERROR_FILE_NOT_FOUND || error == ERROR_PATH_NOT_FOUND ? 0 : -1;
}
bool docBytes(const char *path, std::string &s) {
    std::ifstream in(path, std::ios::binary | std::ios::ate);
    if (!in || in.tellg() < 0 || in.tellg() > static_cast<std::streamoff>(MAX_ANH)) return false;
    s.resize(static_cast<std::size_t>(in.tellg()));
    in.seekg(0);
    if (!s.empty()) in.read(&s[0], static_cast<std::streamsize>(s.size()));
    return bool(in);
}
bool ghiBytes(const char *path, const std::string &s) {
    std::string temp = std::string(path) + ".tmp";
    std::ofstream out(temp.c_str(), std::ios::binary | std::ios::trunc);
    if (!out) return false;
    out.write(s.data(), static_cast<std::streamsize>(s.size()));
    out.close();
    if (!out) return false;
    HANDLE h = CreateFileA(temp.c_str(), GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
    if (h == INVALID_HANDLE_VALUE) return false;
    bool ok = FlushFileBuffers(h) != 0;
    CloseHandle(h);
    return ok && thayFile(temp, path);
}
std::uint32_t checksum(const std::string &s) {
    std::uint32_t h = 2166136261u;
    for (unsigned char c : s) { h ^= c; h *= 16777619u; }
    return h;
}
}

// Phục hồi giao dịch từ file nhật ký pending
bool PhucHoiGiaoDich(const char *dg, const char *ds, std::string &loi) {
    std::string journal = std::string(dg) + ".pending";
    int exists = tonTai(journal.c_str());
    if (!exists) return true;
    loi = "Khong phuc hoi duoc giao dich. Giu nguyen file .pending de thu lai.";
    if (exists < 0) return false;
    try {
        std::string raw, readers, books, tag, header;
        if (!docBytes(journal.c_str(), raw)) return false;
        std::istringstream in(raw);
        if (!std::getline(in, tag) || tag != "LIBRARY_TX1" || !std::getline(in, header)) return false;
        std::istringstream sizes(header);
        std::size_t a = 0, b = 0;
        std::uint32_t ha = 0, hb = 0;
        if (!(sizes >> a >> b >> ha >> hb)) return false;
        sizes >> std::ws;
        if (!sizes.eof() || a > MAX_ANH || b > MAX_ANH || a + b > raw.size()) return false;
        readers.resize(a); books.resize(b);
        if (a) in.read(&readers[0], static_cast<std::streamsize>(a));
        if (b) in.read(&books[0], static_cast<std::streamsize>(b));
        if (!in || in.peek() != std::char_traits<char>::eof() ||
            checksum(readers) != ha || checksum(books) != hb) return false;
        if (!ghiBytes(dg, readers) || !ghiBytes(ds, books)) return false;
        if (std::remove(journal.c_str()) != 0) return false;
        loi.clear();
        return true;
    } catch (const std::exception &) { return false; }
}

// Lưu dữ liệu hai phân hệ an toàn qua file tạm và nhật ký phục hồi
bool LuuDuLieu(TREE_DocGia root, const DanhSachDauSach &ds,
               const char *dgPath, const char *dsPath, std::string &loi) {
    if (!PhucHoiGiaoDich(dgPath, dsPath, loi) || !KiemTraLienKetMuonTra(root, ds, loi)) return false;
    const std::string a = std::string(dgPath) + ".prepare";
    const std::string b = std::string(dsPath) + ".prepare";
    loi = "Luu that bai. Khong bao da luu; kiem tra quyen ghi/dung luong va thu lai.";
    try {
        if (!GhiFileDocGia(root, a.c_str()) || !ghiDauSach(ds, b.c_str())) return false;
        std::string readers, books;
        if (!docBytes(a.c_str(), readers) || !docBytes(b.c_str(), books)) return false;
        std::ostringstream out;
        out << "LIBRARY_TX1\n" << readers.size() << ' ' << books.size() << ' '
            << checksum(readers) << ' ' << checksum(books) << '\n' << readers << books;
        const std::string raw = out.str();
        if (raw.size() > MAX_ANH || !ghiBytes((std::string(dgPath) + ".pending").c_str(), raw)) return false;
        std::remove(a.c_str()); std::remove(b.c_str());
        return PhucHoiGiaoDich(dgPath, dsPath, loi);
    } catch (const std::exception &) { return false; }
}

// Nạp dữ liệu vào bộ nhớ và kiểm tra tính nhất quán
bool NapDuLieu(TREE_DocGia &root, DanhSachDauSach &ds,
               const char *dgPath, const char *dsPath, std::string &loi,
               std::string *chuyenDoi) {
    if (chuyenDoi) chuyenDoi->clear();
    std::string preview;
    if (!PhucHoiGiaoDich(dgPath, dsPath, loi)) return false;
    int a = tonTai(dgPath), b = tonTai(dsPath);
    if (a == 0 && b == 0) return true;
    loi = "File thieu, hong hoac khong doc duoc. Khong ghi de du lieu cu.";
    if (a != 1 || b != 1) return false;
    TREE_DocGia tam = nullptr;
    std::unique_ptr<DanhSachDauSach> books(new DanhSachDauSach{});
    NodeMuonTra *oldArchive = TachLichSuLuuTru();
    bool ok = false;
    try {
        ok = DocFileDocGia(tam, dgPath, &preview) && docDauSach(*books, dsPath) &&
             KiemTraLienKetMuonTra(tam, *books, loi);
    } catch (const std::exception &) { loi = "Khong du tai nguyen de nap du lieu."; }
    if (ok) {
        GiaiPhongCayDocGia(root); giaiPhongDauSach(ds);
        root = tam; tam = nullptr;
        ds = *books; books->n = 0;
        voHieuChiMucISBN();
        GiaiPhongDSMuonTra(oldArchive);
        loi.clear();
        if (chuyenDoi) *chuyenDoi = preview;
    } else ThayLichSuLuuTru(oldArchive);
    GiaiPhongCayDocGia(tam); giaiPhongDauSach(*books);
    return ok;
}
