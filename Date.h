#pragma once
#include "CauTruc.h"
#include <string>
bool namNhuan(int nam);
int soNgayTrongThang(int thang, int nam);
bool ngayHopLe(Date date);
Date ngayHienTaiHeThong();
std::string dinhDangNgay(Date date);
long long soNgayTuyetDoi(Date date);
int khoangCachNgay(Date tu, Date den);
Date congNgay(Date date, int soNgay);
