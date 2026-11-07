#pragma once
#include "CauTruc.h"
#include <string>

// ============================================================================
// PHÂN HỆ: QUẢN LÝ DANH MỤC SÁCH (DSLK ĐƠN)
// NGƯỜI PHỤ TRÁCH: LÊ ANH ĐỨC (N24DECE012)
// ============================================================================

NodeDMS *taoNodeDMS(const char *maSach, int trangThai, const char *viTri);
void khoiTaoDMS(NodeDMS *&head);
bool danhMucSachRong(NodeDMS *head);
bool themCuoiDMS(NodeDMS *&head, NodeDMS *node);
int demSoLuongDMS(NodeDMS *head);
NodeDMS *timKiemDMS(NodeDMS *head, const char *maSach);
void giaiPhongDMS(NodeDMS *&head);
std::string sinhMaSach(const char *ISBN, int stt);
bool themBanSao(DauSach &dauSach, int soLuong, const char *viTri);
bool capNhatTrangThaiSach(NodeDMS *node, int trangThai);
