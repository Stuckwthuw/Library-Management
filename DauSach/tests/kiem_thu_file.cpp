#include "../../DauSach.h"
#include "../../DanhMucSach.h"
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

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

    // File sai không được làm mất danh sách hiện có.
    for(const char *bad: {"DAUSACH_V1\n0\n","DAUSACH_V1 | 0 |", "DAUSACH_V1 | 0 | EXTRA", "DAUSACH_V1 | %XX", "DAUSACH_V1 | 0\n\n"}) {
        ghiThu("bad.txt",bad);assert(!docDauSach(*loaded,"bad.txt")&&loaded->n==1);
    }
    DanhSachDauSach empty{};assert(ghiDauSach(empty,"empty-books.txt"));
    assert(motDong("empty-books.txt")=="DAUSACH_V1 | 0");
    assert(docDauSach(*loaded,"empty-books.txt")&&loaded->n==0);

    giaiPhongDauSach(*ds);giaiPhongDauSach(*loaded);
    std::cout<<"PASS: file mot dong, UTF-8, ky tu |/%, file rong va file loi.\n";
}
