#pragma once
#include "CauTruc.h"
#include <string>
NodeDMS *taoNodeDMS(const char *maSach, int trangThai, const char *viTri);
void khoiTaoDMS(NodeDMS *&head);
bool danhMucSachRong(NodeDMS *head);
// Thành công thì head sở hữu node; từ chối nếu node không độc lập/mã trùng.
bool themCuoiDMS(NodeDMS *&head, NodeDMS *node);
int demSoLuongDMS(NodeDMS *head);
NodeDMS *timKiemDMS(NodeDMS *head, const char *maSach);
void giaiPhongDMS(NodeDMS *&head);
std::string sinhMaSach(const char *ISBN, int stt);
bool themBanSao(DauSach &dauSach, int soLuong, const char *viTri);
// Chỉ kiểm tra trạng thái 0/1/2; điều kiện mượn/trả do module giao dịch quyết định.
bool capNhatTrangThaiSach(NodeDMS *node, int trangThai);
