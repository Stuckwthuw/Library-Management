#include "../DauSach.h"
#include "../DanhMucSach.h"
#include "../FileUtils.h"
#include <memory>
#include <cstdlib>
bool ghiDauSach(const DanhSachDauSach &ds,const char *path) {
    std::ostringstream out;
    out<<"DAUSACH_V1\n"<<ds.n<<'\n';
    for(int i=0;i<ds.n;++i) {
        const DauSach &d=*ds.nodes[i];
        out<<d.ISBN<<'\n'<<d.tenSach<<'\n'<<d.soTrang<<'\n'<<d.tacGia<<'\n'<<d.namXuatBan<<'\n'<<d.theLoai<<'\n'<<demSoLuongDMS(d.dms)<<'\n';
        for(NodeDMS *p=d.dms;p;p=p->pNext)out<<p->data.maSach<<'\n'<<p->data.trangThai<<'\n'<<p->data.viTri<<'\n';
    }
    return bool(out)&&ghiDuLieuMotDong(out.str(),path);
}
bool docDauSach(DanhSachDauSach &ds,const char *path) {
    std::string fields;if(!docDuLieuFile(path,fields))return false;
    std::istringstream in(fields);std::string tag;int n;
    if(!std::getline(in,tag)||tag!="DAUSACH_V1"||!docSo(in,n)||n<0||n>MAX_DAUSACH)return false;
    std::unique_ptr<DanhSachDauSach> tmp(new DanhSachDauSach{});
    try {
        for(int i=0;i<n;++i) {
            std::unique_ptr<DauSach> d(new DauSach{});int count;
            if(!docChuoi(in,d->ISBN)||std::strpbrk(d->ISBN,"_ \t")||!docChuoi(in,d->tenSach)||!docSo(in,d->soTrang)||d->soTrang<1||
               !docChuoi(in,d->tacGia)||!docSo(in,d->namXuatBan)||d->namXuatBan<1||!docChuoi(in,d->theLoai)||!docSo(in,count)||count<0)
                throw 1;
            if(!themDauSach(*tmp,d.get()))throw 1;
            DauSach *added=d.release();NodeDMS *tail=nullptr;
            for(int j=0;j<count;++j){
                DanhMucSach item{};
                if(!docChuoi(in,item.maSach)||!docSo(in,item.trangThai)||item.trangThai<0||item.trangThai>2||!docChuoi(in,item.viTri))throw 1;
                const std::string prefix=std::string(added->ISBN)+"_";
                if(std::string(item.maSach).compare(0,prefix.size(),prefix)!=0)throw 1;
                char *end=nullptr;long k=std::strtol(item.maSach+prefix.size(),&end,10);
                if(!end||*end||k<1||k>2147483647L||sinhMaSach(added->ISBN,static_cast<int>(k))!=item.maSach||timKiemDMS(added->dms,item.maSach))throw 1;
                NodeDMS *p=taoNodeDMS(item.maSach,item.trangThai,item.viTri);
                if(tail)tail->pNext=p;else added->dms=p;tail=p;
            }
        }
        in>>std::ws;if(!in.eof())throw 1;
    } catch (...) {giaiPhongDauSach(*tmp);return false;}
    giaiPhongDauSach(ds);ds=*tmp;voHieuChiMucISBN();return true;
}
