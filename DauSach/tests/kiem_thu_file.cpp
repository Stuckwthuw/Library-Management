#include "../../DauSach.h"
#include "../../DanhMucSach.h"
#include "../../DocGia.h"
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

// Bộ nối BST tối thiểu chỉ dành cho kiểm thử độc lập phần lưu trữ.
// Khi tích hợp thực tế, themDocGia do module độc giả cung cấp.
bool themDocGia(TREE_DocGia &root,const TheDocGia &d) {
    TREE_DocGia *p=&root;
    while(*p) {
        if((*p)->data.maThe==d.maThe)return false;
        p=d.maThe<(*p)->data.maThe?&(*p)->pLeft:&(*p)->pRight;
    }
    *p=new NodeDocGia{d,nullptr,nullptr};return true;
}

namespace {
std::string motDong(const char *path) {
    std::ifstream in(path);std::string line,extra;
    assert(std::getline(in,line)&&!std::getline(in,extra));return line;
}
void ghiThu(const char *path,const char *text) {
    std::ofstream out(path);out<<text;out.close();assert(out);
}
}
int main() {
    std::unique_ptr<DanhSachDauSach> ds(new DanhSachDauSach{}),loaded(new DanhSachDauSach{});
    std::unique_ptr<DauSach> d(new DauSach{});
    std::strcpy(d->ISBN,"CS101");std::strcpy(d->tenSach,"Cấu trúc | dữ liệu 100%");
    std::strcpy(d->tacGia,"Nguyễn Văn An");std::strcpy(d->theLoai,"Tin học");
    d->soTrang=300;d->namXuatBan=2025;
    assert(themBanSao(*d,2,"Kệ A1"));assert(themDauSach(*ds,d.get()));d.release();
    assert(capNhatTrangThaiSach(ds->nodes[0]->dms,1));
    assert(ghiDauSach(*ds,"books.txt"));
    const std::string books=motDong("books.txt");
    assert(books.find("DAUSACH_V1 | 1 | CS101 | ")==0);
    assert(books.find("%7C")!=std::string::npos&&books.find("%25")!=std::string::npos);
    assert(docDauSach(*loaded,"books.txt")&&loaded->n==1);
    assert(!std::strcmp(ds->nodes[0]->tenSach,loaded->nodes[0]->tenSach));
    assert(demSoLuongDMS(loaded->nodes[0]->dms)==2);

    TREE_DocGia root=nullptr,readers=nullptr;
    for(int id: {2,1,3}) {
        TheDocGia dg{};dg.maThe=id;dg.trangThai=1;
        std::strcpy(dg.ho,"Nguyễn Văn");std::strcpy(dg.ten,"An");std::strcpy(dg.phai,"Nam");
        assert(themDocGia(root,dg));
    }
    root->data.dsMuonTra=new NodeMuonTra{};
    MuonTra &m=root->data.dsMuonTra->data;
    std::strcpy(m.maSach,"CS101_1");m.ngayMuon=Date{17,10,2026};
    lichSuTheDaXoa=new NodeMuonTra{};
    MuonTra &old=lichSuTheDaXoa->data;
    std::strcpy(old.maSach,"CS101_2");old.ngayMuon=Date{1,10,2026};
    old.ngayTra=Date{2,10,2026};old.trangThai=1;
    assert(ghiDocGia(root,8,"readers.txt"));
    const std::string readerText=motDong("readers.txt");
    assert(readerText.find("DOCGIA_V2 | 8 | 3 | 2 | ")==0);
    int maxId=0;assert(docDocGia(readers,maxId,"readers.txt")&&maxId==8);
    assert(readers->data.maThe==2&&readers->pLeft->data.maThe==1&&readers->pRight->data.maThe==3);
    assert(lichSuTheDaXoa&&lichSuTheDaXoa->data.trangThai==1);
    assert(kiemTraLienKetMuonTra(readers,*loaded));
    assert(ghiDocGia(readers,maxId,"readers-again.txt"));
    assert(motDong("readers-again.txt")==readerText);

    // File sai không được làm mất cây, danh sách hoặc mã đã cấp hiện có.
    for(const char *bad: {"DAUSACH_V1\n0\n","DAUSACH_V1 | 0 |", "DAUSACH_V1 | 0 | EXTRA", "DAUSACH_V1 | %XX", "DAUSACH_V1 | 0\n\n"}) {
        ghiThu("bad.txt",bad);assert(!docDauSach(*loaded,"bad.txt")&&loaded->n==1);
    }
    for(const char *bad: {"DOCGIA_V2\n0\n0\n0\n","DOCGIA_V1 | 0 | 0","DOCGIA_V2 | 8 | 0", "DOCGIA_V2 | 8 | 0 | 0 | EXTRA"}) {
        ghiThu("bad.txt",bad);assert(!docDocGia(readers,maxId,"bad.txt")&&maxId==8&&readers->data.maThe==2);
    }
    DanhSachDauSach empty{};assert(ghiDauSach(empty,"empty-books.txt"));
    assert(motDong("empty-books.txt")=="DAUSACH_V1 | 0");
    assert(docDauSach(*loaded,"empty-books.txt")&&loaded->n==0);
    giaiPhongMuonTra(lichSuTheDaXoa);
    assert(ghiDocGia(nullptr,8,"empty-readers.txt"));
    assert(motDong("empty-readers.txt")=="DOCGIA_V2 | 8 | 0 | 0");
    assert(docDocGia(readers,maxId,"empty-readers.txt")&&!readers&&maxId==8);
    giaiPhongDocGia(root);giaiPhongDocGia(readers);
    giaiPhongDauSach(*ds);giaiPhongDauSach(*loaded);
    std::cout<<"PASS: file mot dong, UTF-8, ky tu |/%, NLR, lich su da xoa, file rong va file loi.\n";
}
