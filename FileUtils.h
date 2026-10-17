#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdio>
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif
// Một file là một dòng; mã hóa ký tự dành riêng để không tách nhầm nội dung.
inline std::string maHoaTruong(const std::string &s) {
    std::string result;
    for(char c:s) {
        if(c=='%')result+="%25";
        else if(c=='|')result+="%7C";
        else if(c=='\n')result+="%0A";
        else if(c=='\r')result+="%0D";
        else result+=c;
    }
    return result;
}
inline bool giaiMaTruong(const std::string &s,std::string &result) {
    result.clear();
    for(std::size_t i=0;i<s.size();++i) {
        if(s[i]!='%'){result+=s[i];continue;}
        const std::string code=s.substr(i,3);
        if(code=="%25")result+='%';
        else if(code=="%7C")result+='|';
        else if(code=="%0A")result+='\n';
        else if(code=="%0D")result+='\r';
        else return false;
        i+=2;
    }
    return true;
}
inline bool thayFile(const std::string &temp,const char *dest);
inline bool ghiDuLieuMotDong(const std::string &fields,const char *path) {
    const std::string temp=std::string(path)+".tmp";
    std::ofstream out(temp.c_str(),std::ios::trunc);
    if(!out)return false;
    std::istringstream input(fields);std::string field;bool first=true;
    while(std::getline(input,field)) {
        if(!first)out<<" | ";
        out<<maHoaTruong(field);first=false;
    }
    out<<'\n';out.close();
    return bool(out)&&thayFile(temp,path);
}
// Chỉ nhận định dạng một dòng có dấu |; dùng luồng trường để kiểm tra dữ liệu.
inline bool docDuLieuFile(const char *path,std::string &fields) {
    std::ifstream input(path);if(!input)return false;
    std::ostringstream buffer;buffer<<input.rdbuf();
    if(input.bad())return false;
    std::string text=buffer.str();
    if(!text.empty()&&text.back()=='\n')text.pop_back();
    if(!text.empty()&&text.back()=='\r')text.pop_back();
    if(text.find('|')==std::string::npos||text.find_first_of("\r\n")!=std::string::npos)return false;
    fields.clear();std::size_t start=0;
    for(;;) {
        const std::size_t end=text.find('|',start);
        std::string field=text.substr(start,end==std::string::npos?end:end-start);
        const std::size_t first=field.find_first_not_of(" \t");
        if(first==std::string::npos)return false;
        field=field.substr(first,field.find_last_not_of(" \t")-first+1);
        std::string decoded;if(!giaiMaTruong(field,decoded))return false;
        // Xuống dòng không hợp lệ trong trường của định dạng hiện tại.
        if(decoded.find_first_of("\r\n")!=std::string::npos)return false;
        fields+=decoded+'\n';
        if(end==std::string::npos)break;
        start=end+1;
    }
    return true;
}
inline bool docSo(std::istream &in,int &n) {
    std::string s; if(!std::getline(in,s))return false;
    std::istringstream ss(s);char extra;return bool(ss>>n)&&!(ss>>extra);
}
template<std::size_t N> bool docChuoi(std::istream &in,char (&s)[N],bool rong=false) {
    std::string line;if(!std::getline(in,line)||line.size()>=N||(!rong&&line.empty()))return false;
    std::strcpy(s,line.c_str());return true;
}
inline bool thayFile(const std::string &temp,const char *dest) {
#ifdef _WIN32
    return MoveFileExA(temp.c_str(),dest,MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH)!=0;
#else
    return std::rename(temp.c_str(),dest)==0;
#endif
}
