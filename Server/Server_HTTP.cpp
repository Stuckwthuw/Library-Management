#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <wincrypt.h>
#include "../Server.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cctype>
namespace WebServer {
namespace {
std::string lower(std::string s) { for(char &c:s) c=static_cast<char>(std::tolower(static_cast<unsigned char>(c))); return s; }
std::string header(const std::string &h,const std::string &key) {
    std::istringstream in(h); std::string line,result; bool found=false;
    std::getline(in,line);
    while(std::getline(in,line)) {
        if(!line.empty()&&line.back()=='\r') line.pop_back();
        auto colon=line.find(':');
        if(colon!=std::string::npos && lower(line.substr(0,colon))==key) {
            if(found) throw std::invalid_argument("Duplicate header");
            result=line.substr(colon+1); found=true;
            while(!result.empty()&&result.front()==' ') result.erase(0,1);
            while(!result.empty()&&result.back()==' ') result.pop_back();
        }
    }
    return result;
}
void sendReply(SOCKET socket,int status,const std::string &body,const std::string &type="application/json; charset=utf-8") {
    std::string data="HTTP/1.1 "+std::to_string(status)+" Response\r\nContent-Type: "+type+"\r\nContent-Length: "+std::to_string(body.size())+"\r\nConnection: close\r\nCache-Control: no-store\r\nX-Content-Type-Options: nosniff\r\nContent-Security-Policy: default-src 'self'; style-src 'self'; script-src 'self'; frame-ancestors 'none'\r\n\r\n"+body;
    for(std::size_t p=0;p<data.size();) {
        int count=send(socket,data.data()+p,static_cast<int>(data.size()-p),0);
        if(count<=0) break;
        p+=count;
    }
}
// Một request/kết nối, đọc từng phần; giới hạn bộ nhớ và thời gian để bảo vệ server tuần tự.
void serve(SOCKET socket,State &s,int port) {
    DWORD timeout=2000;
    setsockopt(socket,SOL_SOCKET,SO_RCVTIMEO,reinterpret_cast<const char*>(&timeout),sizeof(timeout));
    setsockopt(socket,SOL_SOCKET,SO_SNDTIMEO,reinterpret_cast<const char*>(&timeout),sizeof(timeout));
    ULONGLONG start=GetTickCount64(); std::string raw; char buffer[4096];
    auto receive=[&]() {
        if(GetTickCount64()-start>5000) throw std::invalid_argument("Timeout");
        int count=recv(socket,buffer,sizeof(buffer),0);
        if(count<=0) throw std::invalid_argument("Incomplete request");
        raw.append(buffer,count);
        if(raw.size()>73728) throw std::invalid_argument("Request too large");
    };
    try {
        while(raw.find("\r\n\r\n")==std::string::npos) { receive(); if(raw.size()>8192) throw std::invalid_argument("Header too large"); }
        auto end=raw.find("\r\n\r\n"); if(end>8192) throw std::invalid_argument("Header too large");
        std::string h=raw.substr(0,end), method,target,protocol,extra;
        std::istringstream line(h.substr(0,h.find("\r\n"))); line>>method>>target>>protocol;
        if(protocol!="HTTP/1.1" || (line>>extra) || target.empty() || target[0]!='/') throw std::invalid_argument("Bad request line");
        std::string host=header(h,"host"), origin=header(h,"origin");
        std::string expected="127.0.0.1:"+std::to_string(port), local="localhost:"+std::to_string(port);
        if((host!=expected&&host!=local)||(!origin.empty()&&origin!="http://"+host)) { sendReply(socket,403,"{}"); return; }
        if(!header(h,"transfer-encoding").empty()) throw std::invalid_argument("Unsupported transfer encoding");
        int length=header(h,"content-length").empty()?0:number(header(h,"content-length"),0,65536);
        while(raw.size()<end+4+length) receive();
        auto query=target.find('?'); std::string path=target.substr(0,query);
        if(path.compare(0,8,"/api/v1/")==0) {
            if(method=="POST" && header(h,"content-type")!="application/x-www-form-urlencoded") { sendReply(socket,415,"{}"); return; }
            std::string form=method=="POST"?raw.substr(end+4,length):query==std::string::npos?"":target.substr(query+1);
            Reply reply=api(s,method,path,form); sendReply(socket,reply.status,reply.body);
        } else if(method=="GET" && (path=="/"||path=="/app.js"||path=="/style.css")) {
            std::ifstream file("Web/"+(path=="/"?std::string("index.html"):path.substr(1)),std::ios::binary);
            if(!file) { sendReply(socket,404,"{}"); return; }
            std::ostringstream out; out<<file.rdbuf();
            sendReply(socket,200,out.str(),path=="/"?"text/html; charset=utf-8":path=="/app.js"?"text/javascript; charset=utf-8":"text/css; charset=utf-8");
        } else sendReply(socket,404,"{}");
    } catch(const std::exception &) { sendReply(socket,400,"{\"ok\":false,\"message\":\"HTTP không hợp lệ\"}"); }
}
}
int run(State &s,int port) {
    HCRYPTPROV provider=0; unsigned char bytes[32];
    if(!CryptAcquireContext(&provider,nullptr,nullptr,PROV_RSA_FULL,CRYPT_VERIFYCONTEXT)) return 2;
    bool random=CryptGenRandom(provider,sizeof(bytes),bytes)!=0; CryptReleaseContext(provider,0);
    if(!random) return 2;
    const char *hex="0123456789abcdef";
    for(unsigned char b:bytes) { s.token+=hex[b>>4]; s.token+=hex[b&15]; }
    WSADATA wsa; if(WSAStartup(MAKEWORD(2,2),&wsa)) return 2;
    SOCKET listener=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(listener==INVALID_SOCKET) { WSACleanup(); return 2; }
    BOOL exclusive=TRUE; setsockopt(listener,SOL_SOCKET,SO_EXCLUSIVEADDRUSE,reinterpret_cast<const char*>(&exclusive),sizeof(exclusive));
    sockaddr_in address{}; address.sin_family=AF_INET; address.sin_addr.s_addr=htonl(INADDR_LOOPBACK); address.sin_port=htons(static_cast<u_short>(port));
    if(bind(listener,reinterpret_cast<sockaddr*>(&address),sizeof(address))==SOCKET_ERROR || listen(listener,8)==SOCKET_ERROR) {
        closesocket(listener); WSACleanup(); return 3;
    }
    while(!s.stopping) {
        SOCKET client=accept(listener,nullptr,nullptr);
        if(client==INVALID_SOCKET) break;
        serve(client,s,port); closesocket(client);
    }
    closesocket(listener); WSACleanup(); return 0;
}
}
