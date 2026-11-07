#pragma once
#include "../CauTruc.h"
#include <string>

// Kiểm tra tính nhất quán liên kết giữa độc giả và sách
bool KiemTraLienKetMuonTra(TREE_DocGia root, const DanhSachDauSach &ds,
                          std::string &loi);

// Phục hồi và đồng bộ dữ liệu hai phân hệ
bool PhucHoiGiaoDich(const char *docGia, const char *dauSach, std::string &loi);
bool LuuDuLieu(TREE_DocGia root, const DanhSachDauSach &ds,
               const char *docGia, const char *dauSach, std::string &loi);
bool NapDuLieu(TREE_DocGia &root, DanhSachDauSach &ds,
               const char *docGia, const char *dauSach, std::string &loi);
