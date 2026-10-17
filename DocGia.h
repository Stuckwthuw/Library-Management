#pragma once
#include "CauTruc.h"
bool themDocGia(TREE_DocGia &root,const TheDocGia &docGia);
void giaiPhongMuonTra(NodeMuonTra *&head);
void giaiPhongDocGia(TREE_DocGia &root);
bool ghiDocGia(TREE_DocGia root,int maDaCap,const char *path);
bool docDocGia(TREE_DocGia &root,int &maDaCap,const char *path);
bool kiemTraLienKetMuonTra(TREE_DocGia root,const DanhSachDauSach &ds);
// Lich su cua the da xoa: chi co luot da tra / lam mat, dung cho thong ke.
extern NodeMuonTra *lichSuTheDaXoa;
void luuLichSuTheDaXoa(NodeMuonTra *&head);
