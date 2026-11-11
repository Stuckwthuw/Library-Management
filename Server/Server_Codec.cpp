#include "../Server.h"
#include "../Common/Chuoi.h"
#include <stdexcept>
namespace WebServer {
// Escape theo byte UTF-8; ký tự điều khiển không được lọt vào chuỗi JSON.
std::string quote(const std::string &s) {
    std::string out = "\"";
    const char *hex = "0123456789abcdef";
    for (unsigned char c : s) {
        if (c == '"' || c == '\\') { out += '\\'; out += c; }
        else if (c < 32) { out += "\\u00"; out += hex[c >> 4]; out += hex[c & 15]; }
        else out += c;
    }
    return out + '"';
}
static int digit(char c) {
    if (c >= '0' && c <= '9') return c-'0';
    if (c >= 'A' && c <= 'F') return c-'A'+10;
    if (c >= 'a' && c <= 'f') return c-'a'+10;
    throw std::invalid_argument("Mã URL không hợp lệ");
}
static std::string decode(const std::string &s) {
    std::string out;
    for (std::size_t i=0;i<s.size();++i) {
        if (s[i]=='%') {
            if (i+2>=s.size()) throw std::invalid_argument("Mã URL bị thiếu");
            char c=static_cast<char>(digit(s[i+1])*16+digit(s[i+2])); i+=2;
            if (!c) throw std::invalid_argument("Không nhận ký tự NUL");
            out+=c;
        } else out += s[i]=='+' ? ' ' : s[i];
    }
    if (!Chuoi::hopLe(out.c_str())) throw std::invalid_argument("UTF-8 không hợp lệ");
    return out;
}
std::string field(const std::string &form, const std::string &name) {
    std::string result; bool found=false;
    for (std::size_t p=0;p<form.size();) {
        auto e=form.find('&',p); if(e==std::string::npos) e=form.size();
        auto eq=form.find('=',p);
        if(eq<e && decode(form.substr(p,eq-p))==name) {
            if(found) throw std::invalid_argument("Trường bị lặp");
            result=decode(form.substr(eq+1,e-eq-1)); found=true;
        }
        p=e+1;
    }
    return result;
}
int number(const std::string &s,int low,int high) {
    if(s.empty()) throw std::invalid_argument("Thiếu số nguyên");
    int n=0;
    for(char c:s) {
        if(c<'0'||c>'9'||n>(high-(c-'0'))/10) throw std::invalid_argument("Số ngoài miền hợp lệ");
        n=n*10+c-'0';
    }
    if(n<low||n>high) throw std::invalid_argument("Số ngoài miền hợp lệ");
    return n;
}
}
