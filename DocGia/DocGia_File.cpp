#include "../DocGia.h"
#include "../DauSach.h"
#include "../FileUtils.h"
#include "../Date.h"
#include <memory>
NodeMuonTra *lichSuTheDaXoa=nullptr;
void luuLichSuTheDaXoa(NodeMuonTra *&head) {
    if(!head)return;
    NodeMuonTra *tail=head;
    while(tail->pNext)tail=tail->pNext;
    tail->pNext=lichSuTheDaXoa;
    lichSuTheDaXoa=head;
    head=nullptr;
}
void giaiPhongMuonTra(NodeMuonTra *&p) {while(p){NodeMuonTra *old=p;p=p->pNext;delete old;}}
void giaiPhongDocGia(TREE_DocGia &root) {
    if(!root)return;
    giaiPhongDocGia(root->pLeft);giaiPhongDocGia(root->pRight);giaiPhongMuonTra(root->data.dsMuonTra);delete root;root=nullptr;
}
namespace {
int demDocGia(TREE_DocGia p){return p?1+demDocGia(p->pLeft)+demDocGia(p->pRight):0;}
void ghiNLR(std::ostream &out,TREE_DocGia p) {
    if(!p)return;
    const TheDocGia &d=p->data;
    out<<d.maThe<<'\n'<<d.ho<<'\n'<<d.ten<<'\n'<<d.phai<<'\n'<<d.trangThai<<'\n';
    int n=0;for(NodeMuonTra *m=d.dsMuonTra;m;m=m->pNext)++n;out<<n<<'\n';
    for(NodeMuonTra *m=d.dsMuonTra;m;m=m->pNext){const MuonTra &x=m->data;
        out<<x.maSach<<'\n'<<x.ngayMuon.ngay<<' '<<x.ngayMuon.thang<<' '<<x.ngayMuon.nam<<'\n'
           <<x.ngayTra.ngay<<' '<<x.ngayTra.thang<<' '<<x.ngayTra.nam<<'\n'<<x.trangThai<<'\n';}
    ghiNLR(out,p->pLeft);ghiNLR(out,p->pRight);
}
bool docNgay(std::istream &in,Date &d){std::string s;if(!std::getline(in,s))return false;std::istringstream ss(s);char extra;return bool(ss>>d.ngay>>d.thang>>d.nam)&&!(ss>>extra);}
int demDangMuon(TREE_DocGia root,const char *ma){
    if(!root)return 0;
    int n=0;
    for(NodeMuonTra *p=root->data.dsMuonTra;p;p=p->pNext)if(p->data.trangThai==0&&!std::strcmp(p->data.maSach,ma))++n;
    return n+demDangMuon(root->pLeft,ma)+demDangMuon(root->pRight,ma);
}
bool kiemTraLichSu(TREE_DocGia root,const DanhSachDauSach &ds){
    if(!root)return true;
    int dangMuon=0;
    for(NodeMuonTra *m=root->data.dsMuonTra;m;m=m->pNext){DauSach *d;NodeDMS *p;
        if(!timSachTheoMaSach(ds,m->data.maSach,d,p))return false;
        if(m->data.trangThai==0){++dangMuon;if(p->data.trangThai!=1)return false;}
        if(m->data.trangThai==2&&p->data.trangThai!=2)return false;
    }
    return dangMuon<=MAX_MUON_SACH&&kiemTraLichSu(root->pLeft,ds)&&kiemTraLichSu(root->pRight,ds);
}
}
bool ghiDocGia(TREE_DocGia root,int maDaCap,const char *path){
    std::ostringstream out;
    out<<"DOCGIA_V2\n"<<maDaCap<<'\n'<<demDocGia(root)<<'\n';
    ghiNLR(out,root);
    int n=0;
    for(NodeMuonTra *p=lichSuTheDaXoa;p;p=p->pNext)++n;
    out<<n<<'\n';
    for(NodeMuonTra *p=lichSuTheDaXoa;p;p=p->pNext) {
        const MuonTra &x=p->data;
        out<<x.maSach<<'\n'<<x.ngayMuon.ngay<<' '<<x.ngayMuon.thang<<' '<<x.ngayMuon.nam<<'\n'
           <<x.ngayTra.ngay<<' '<<x.ngayTra.thang<<' '<<x.ngayTra.nam<<'\n'<<x.trangThai<<'\n';
    }
    return bool(out)&&ghiDuLieuMotDong(out.str(),path);
}
bool docDocGia(TREE_DocGia &root,int &maDaCap,const char *path){
    std::string fields;if(!docDuLieuFile(path,fields))return false;
    std::istringstream in(fields);std::string tag;int maxId,n;
    if(!std::getline(in,tag)||tag!="DOCGIA_V2"||!docSo(in,maxId)||maxId<0||!docSo(in,n)||n<0)return false;
    TREE_DocGia tmp=nullptr;
    NodeMuonTra *archive=nullptr;
    try {
        for(int i=0;i<n;++i){TheDocGia d{};int count;
            if(!docSo(in,d.maThe)||d.maThe<1||d.maThe>maxId||!docChuoi(in,d.ho)||!docChuoi(in,d.ten)||!docChuoi(in,d.phai)||
               (std::strcmp(d.phai,"Nam")&&std::strcmp(d.phai,"Nữ"))||!docSo(in,d.trangThai)||d.trangThai<0||d.trangThai>1||!docSo(in,count)||count<0)throw 1;
            // So huu tam danh sach trong luc doc; chi chuyen cho cay khi doc du.
            NodeMuonTra *head=nullptr,*tail=nullptr;
            try {
                for(int j=0;j<count;++j){std::unique_ptr<NodeMuonTra> m(new NodeMuonTra{});MuonTra &x=m->data;
                    if(!docChuoi(in,x.maSach)||!docNgay(in,x.ngayMuon)||!ngayHopLe(x.ngayMuon)||!docNgay(in,x.ngayTra)||!docSo(in,x.trangThai)||x.trangThai<0||x.trangThai>2)throw 1;
                    if(x.trangThai==0){if(x.ngayTra.ngay||x.ngayTra.thang||x.ngayTra.nam)throw 1;}
                    else if(!ngayHopLe(x.ngayTra)||khoangCachNgay(x.ngayMuon,x.ngayTra)<0)throw 1;
                    if(tail)tail->pNext=m.get();else head=m.get();tail=m.release();
                }
                d.dsMuonTra=head;if(!themDocGia(tmp,d))throw 1;
            } catch (...){giaiPhongMuonTra(head);throw;}
        }
        {
            int count;
            if(!docSo(in,count)||count<0)throw 1;
            NodeMuonTra *tail=nullptr;
            for(int j=0;j<count;++j) {
                std::unique_ptr<NodeMuonTra> m(new NodeMuonTra{});
                MuonTra &x=m->data;
                if(!docChuoi(in,x.maSach)||!docNgay(in,x.ngayMuon)||!ngayHopLe(x.ngayMuon)||
                   !docNgay(in,x.ngayTra)||!ngayHopLe(x.ngayTra)||khoangCachNgay(x.ngayMuon,x.ngayTra)<0||
                   !docSo(in,x.trangThai)||x.trangThai<1||x.trangThai>2)throw 1;
                if(tail)tail->pNext=m.get();else archive=m.get();
                tail=m.release();
            }
        }
        in>>std::ws;if(!in.eof())throw 1;
    } catch (...){giaiPhongDocGia(tmp);giaiPhongMuonTra(archive);return false;}
    giaiPhongDocGia(root);root=tmp;maDaCap=maxId;
    giaiPhongMuonTra(lichSuTheDaXoa);lichSuTheDaXoa=archive;
    return true;
}
bool kiemTraLienKetMuonTra(TREE_DocGia root,const DanhSachDauSach &ds){
    if(!kiemTraLichSu(root,ds))return false;
    for(NodeMuonTra *m=lichSuTheDaXoa;m;m=m->pNext) {
        DauSach *d;NodeDMS *p;
        if(!timSachTheoMaSach(ds,m->data.maSach,d,p)||m->data.trangThai==0)return false;
        if(m->data.trangThai==2&&p->data.trangThai!=2)return false;
    }
    for(int i=0;i<ds.n;++i)for(NodeDMS *p=ds.nodes[i]->dms;p;p=p->pNext){
        int n=demDangMuon(root,p->data.maSach);
        if((p->data.trangThai==1&&n!=1)||(p->data.trangThai!=1&&n!=0))return false;
    }
    return true;
}
