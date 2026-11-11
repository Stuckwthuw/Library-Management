#include "Server.h"
#include "Common/LuuTru.h"
#include <windows.h>
#include <iostream>
// Một tiến trình sở hữu dữ liệu, giải phóng tất cả cấu trúc khi dừng server.
int main(int argc,char **argv) {
    int port=8080;
    try { if(argc>1) port=WebServer::number(argv[1],1024,65535); }
    catch(const std::exception &e) { std::cerr<<e.what(); return 1; }
    HANDLE lock=CreateMutexA(nullptr,FALSE,"Local\\LibraryManagementWeb");
    if(!lock||GetLastError()==ERROR_ALREADY_EXISTS) { if(lock) CloseHandle(lock); return 4; }
    CreateDirectoryA("Data",nullptr);
    WebServer::State state;
    state.ready=NapDuLieu(state.readers,state.books,"Data/DocGia.txt","Data/DauSach.txt",state.error,&state.migration);
    std::cerr<<"Web: http://127.0.0.1:"<<port<<"\n";
    int result=WebServer::run(state,port);
    if(result) std::cerr<<"Server error "<<result<<" (check port/resources).\n";
    GiaiPhongCayDocGia(state.readers); giaiPhongDauSach(state.books); GiaiPhongLichSuLuuTru(); CloseHandle(lock);
    return result;
}
