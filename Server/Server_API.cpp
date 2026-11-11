#include "../Server.h"
#include "../MuonTra.h"
#include "../DanhMucSach.h"
#include "../Common/LuuTru.h"
#include <memory>
#include <cstring>
#include <stdexcept>
namespace WebServer {
namespace {
std::string n(int x) { return std::to_string(x); }
template<std::size_t N> void copy(char (&out)[N],const std::string &s,bool title=false,bool code=false) {
    if(s.empty()||s.size()>=N) throw std::invalid_argument("Trường rỗng hoặc quá dài theo byte UTF-8");
    std::strcpy(out,s.c_str());
    if(code) ChuanHoaMa(out); else if(title) DinhDangHoTen(out); else ChuanHoaKhoangTrang(out);
    if(KiemTraChuoiRong(out)) throw std::invalid_argument("Trường không được rỗng");
}
std::string reader(const TheDocGia &d) {
    return "{\"maThe\":"+n(d.maThe)+",\"ho\":"+quote(d.ho)+",\"ten\":"+quote(d.ten)+",\"phai\":"+quote(d.phai)+",\"trangThai\":"+n(d.trangThai)+",\"dangMuon\":"+n(DemSoSachDangMuon(d.dsMuonTra))+"}";
}
std::string book(const DauSach &d) {
    return "{\"ISBN\":"+quote(d.ISBN)+",\"tenSach\":"+quote(d.tenSach)+",\"tacGia\":"+quote(d.tacGia)+",\"theLoai\":"+quote(d.theLoai)+",\"soTrang\":"+n(d.soTrang)+",\"namXuatBan\":"+n(d.namXuatBan)+"}";
}
std::string date(Date d) { return quote(n(d.ngay)+"/"+n(d.thang)+"/"+n(d.nam)); }
bool save(State &s) {
    s.dirty=true;
    if(!LuuDuLieu(s.readers,s.books,"Data/DocGia.txt","Data/DauSach.txt",s.error)) return false;
    s.dirty=false; s.migration.clear(); return true;
}
}
Reply api(State &s,const std::string &method,const std::string &path,const std::string &form) {
    Reply r;
    struct Responder { Reply &r; State &s; Reply operator()(int status,const std::string &code,const std::string &message,const std::string &data="null") {
        r.status=status; r.body="{\"ok\":"+std::string(status<300?"true":"false")+",\"code\":"+quote(code)+",\"message\":"+quote(message)+",\"version\":"+n(s.version)+",\"data\":"+data+"}"; return r;
    }} respond{r,s};
    try {
        auto f=[&](const char *key){return field(form,key);};
        if(method=="GET" && path=="/api/v1/status") return respond(200,"OK","", "{\"ready\":"+std::string(s.ready?"true":"false")+",\"dirty\":"+(s.dirty?"true":"false")+",\"error\":"+quote(s.error)+",\"migration\":"+quote(s.migration)+",\"token\":"+quote(s.token)+"}");
        if(method=="POST") {
            if(f("token")!=s.token) return respond(403,"TOKEN","Phiên không hợp lệ");
            if(f("version")!=n(s.version)) return respond(409,"STALE","Dữ liệu đã thay đổi. Tải lại trước khi xác nhận.");
            if(path=="/api/v1/stop") {
                if(s.dirty) return respond(409,"UNSAVED","Cần thử lưu lại trước khi dừng");
                s.stopping=true; return respond(200,"OK","Đã dừng ứng dụng");
            }
            if(path=="/api/v1/save" || path=="/api/v1/migrate") {
                if(!s.ready) return respond(503,"LOAD","Không thể nạp dữ liệu");
                ++s.version;
                return save(s)?respond(200,"OK","Đã lưu dữ liệu"):respond(503,"SAVE",s.error);
            }
        }
        if(!s.ready||s.dirty||!s.migration.empty()) return respond(503,"BLOCKED",s.error.empty()?"Cần xác nhận chuyển đổi hoặc lưu lại dữ liệu":s.error);
        if(method=="GET") {
            int page=f("page").empty()?1:number(f("page"),1,1000000);
            int size=f("size").empty()?20:number(f("size"),1,100);
            int total=0; std::string rows;
            auto add=[&](const std::string &row) { if(total>=(page-1)*size && total<page*size) { if(!rows.empty()) rows+=','; rows+=row; } ++total; };
            if(path=="/api/v1/readers") {
                int count=DemSoLuongDocGia(s.readers), i=0;
                std::unique_ptr<TheDocGia*[]> a(new TheDocGia*[count]); DoCayVaoMang(s.readers,a.get(),i);
                if(f("sort")=="name") QuickSortDocGia(a.get(),0,count-1);
                for(i=0;i<count;++i) if(f("q").empty()||n(a[i]->maThe)==f("q")||ChuaChuoiConKhongPhanBietHoaThuong((std::string(a[i]->ho)+" "+a[i]->ten).c_str(),f("q").c_str())) add(reader(*a[i]));
            } else if(path=="/api/v1/books") {
                std::unique_ptr<DauSach*[]> a(new DauSach*[MAX_DAUSACH]);
                int count=layDauSachTheoTheLoai(s.books,a.get(),MAX_DAUSACH);
                for(int i=0;i<count;++i) if((f("q").empty()||tenChuaTuKhoa(a[i]->tenSach,f("q"))||f("q")==a[i]->ISBN) && (f("genre").empty()||f("genre")==a[i]->theLoai)) add(book(*a[i]));
            } else if(path=="/api/v1/copies") {
                DauSach *d=timDauSachTheoISBN(s.books,f("isbn").c_str());
                if(!d) return respond(404,"NOT_FOUND","Không tìm thấy đầu sách");
                for(NodeDMS *p=d->dms;p;p=p->pNext) add("{\"maSach\":"+quote(p->data.maSach)+",\"tenSach\":"+quote(d->tenSach)+",\"viTri\":"+quote(p->data.viTri)+",\"trangThai\":"+n(p->data.trangThai)+"}");
            } else if(path=="/api/v1/loans") {
                NodeDocGia *d=TimKiemDocGia(s.readers,number(f("id"),1,999999));
                if(!d) return respond(404,"NOT_FOUND","Không tìm thấy độc giả");
                for(NodeMuonTra *p=d->data.dsMuonTra;p;p=p->pNext) if(p->data.trangThai==0) {
                    DauSach *b=nullptr; NodeDMS *c=nullptr; timSachTheoMaSach(s.books,p->data.maSach,b,c);
                    add("{\"maSach\":"+quote(p->data.maSach)+",\"tenSach\":"+quote(b?b->tenSach:"")+",\"viTri\":"+quote(c?c->data.viTri:"")+",\"ngayMuon\":"+date(p->data.ngayMuon)+"}");
                }
            } else if(path=="/api/v1/preview") {
                NodeDocGia *d=TimKiemDocGia(s.readers,number(f("id"),1,999999));
                DauSach *b=nullptr; NodeDMS *c=nullptr;
                char code[20]; copy(code,f("code"),false,true);
                if(!d||!timSachTheoMaSach(s.books,code,b,c)) return respond(404,"NOT_FOUND","Không tìm thấy độc giả hoặc sách");
                return respond(200,"OK","","{\"docGia\":"+reader(d->data)+",\"tenSach\":"+quote(b->tenSach)+",\"maSach\":"+quote(code)+",\"viTri\":"+quote(c->data.viTri)+"}");
            } else if(path=="/api/v1/overdue") {
                QuaHanInfo *raw=nullptr; int count=LapDanhSachQuaHan(s.readers,raw); std::unique_ptr<QuaHanInfo[]> a(raw);
                for(int i=0;i<count;++i) add("{\"maThe\":"+n(a[i].maThe)+",\"hoTen\":"+quote(std::string(a[i].ho)+" "+a[i].ten)+",\"maSach\":"+quote(a[i].maSach)+",\"ngayMuon\":"+date(a[i].ngayMuon)+",\"quaHan\":"+n(a[i].soNgayQuaHan)+"}");
            } else if(path=="/api/v1/top") {
                ThongKeSach a[10]; int count=layTop10(s.books,s.readers,a,10,LayLichSuLuuTru());
                for(int i=0;i<count;++i) add("{\"ISBN\":"+quote(a[i].sach->ISBN)+",\"tenSach\":"+quote(a[i].sach->tenSach)+",\"luotMuon\":"+n(a[i].soLuotMuon)+"}");
            } else return respond(404,"NOT_FOUND","API không tồn tại");
            return respond(200,"OK","","{\"items\":["+rows+"],\"total\":"+n(total)+",\"page\":"+n(page)+"}");
        }
        if(method!="POST") return respond(405,"METHOD","Phương thức không hỗ trợ");
        bool ok=false;
        if(path=="/api/v1/readers/add"||path=="/api/v1/readers/edit") {
            TheDocGia d{}; copy(d.ho,f("ho"),true); copy(d.ten,f("ten"),true); copy(d.phai,f("phai")); d.trangThai=number(f("status"),0,1);
            if(!KiemTraTenMotTu(d.ten)) return respond(400,"TEN","Tên chỉ gồm một từ; chuyển phần còn lại vào họ và tên đệm.");
            if(!KiemTraChuCaiVaKhoangTrang(d.ho)) return respond(400,"HO","Họ và tên đệm chỉ nhận chữ cái và khoảng trắng.");
            if(path=="/api/v1/readers/add") { d.maThe=SinhMaTheTuDong(s.readers); ok=ThemDocGia(s.readers,d); }
            else ok=HieuChinhDocGia(s.readers,number(f("id"),1,999999),d.ho,d.ten,d.phai,d.trangThai);
        } else if(path=="/api/v1/readers/delete") ok=XoaDocGia(s.readers,number(f("id"),1,999999));
        else if(path=="/api/v1/books/add") {
            std::unique_ptr<DauSach> d(new DauSach{});
            copy(d->ISBN,f("isbn"),false,true); copy(d->tenSach,f("title")); copy(d->tacGia,f("author"),true); copy(d->theLoai,f("genre"));
            if(!KiemTraChuCaiVaKhoangTrang(d->tacGia)||!KiemTraChuCaiVaKhoangTrang(d->theLoai)) throw std::invalid_argument("Tác giả và thể loại phải chứa chữ cái");
            d->soTrang=number(f("pages"),1,100000); d->namXuatBan=number(f("year"),1000,2100);
            ok=themDauSach(s.books,d.get()); if(ok) d.release();
        } else if(path=="/api/v1/copies/add") {
            char isbn[15], shelf[50]; copy(isbn,f("isbn"),false,true); copy(shelf,f("shelf"));
            DauSach *d=timDauSachTheoISBN(s.books,isbn); if(d) ok=themBanSao(*d,number(f("count"),1,1000),shelf);
        } else if(path=="/api/v1/borrow"||path=="/api/v1/return"||path=="/api/v1/lost") {
            char code[20]; copy(code,f("code"),false,true); int id=number(f("id"),1,999999);
            NodeDocGia *d=TimKiemDocGia(s.readers,id);
            DauSach *b=nullptr; NodeDMS *c=nullptr;
            if(!d||!timSachTheoMaSach(s.books,code,b,c)) return respond(404,"NOT_FOUND","Không tìm thấy độc giả hoặc mã sách.");
            if(path=="/api/v1/borrow") {
                KetQuaKiemTraMuon check=KiemTraDieuKienMuon(d);
                if(check==LOI_THE_BI_KHOA) return respond(409,"CARD_LOCKED","Thẻ độc giả đang bị khóa.");
                if(check==LOI_DA_MUON_TOI_DA) return respond(409,"LIMIT","Độc giả đã giữ tối đa 3 cuốn.");
                if(check==LOI_CO_SACH_QUA_HAN) return respond(409,"OVERDUE","Độc giả có sách quá hạn; cần trả trước khi mượn tiếp.");
                for(NodeMuonTra *p=d->data.dsMuonTra;p;p=p->pNext)
                    if(p->data.trangThai==0&&CungISBN(p->data.maSach,code)) return respond(409,"ISBN_DUPLICATE","Độc giả đang giữ một cuốn cùng ISBN.");
                if(c->data.trangThai!=0) return respond(409,"UNAVAILABLE","Cuốn sách đã được mượn hoặc thanh lý.");
            }
            ok=path=="/api/v1/borrow"?MuonSach(s.readers,s.books,id,code):path=="/api/v1/return"?TraSach(s.readers,s.books,id,code):BaoMatSach(s.readers,s.books,id,code);
        } else return respond(404,"NOT_FOUND","API không tồn tại");
        if(!ok) return respond(409,"RULE","Thao tác bị từ chối: kiểm tra dữ liệu, trạng thái thẻ/sách, giới hạn mượn, quá hạn và ISBN trùng.");
        ++s.version;
        return save(s)?respond(200,"OK","Đã cập nhật và lưu dữ liệu"):respond(503,"SAVE",s.error);
    } catch(const std::invalid_argument &e) { return respond(400,"INPUT",e.what()); }
    catch(const std::exception &) { return respond(500,"INTERNAL","Không đủ tài nguyên để xử lý"); }
}
}
