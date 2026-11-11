#pragma once
#include "DocGia.h"
#include "DauSach.h"
#include <string>
namespace WebServer {
struct State {
    TREE_DocGia readers = nullptr;
    DanhSachDauSach books{};
    std::string error, migration, token;
    unsigned version = 1;
    bool ready = false, dirty = false, stopping = false;
};
struct Reply { int status = 200; std::string body; };
std::string quote(const std::string &s);
std::string field(const std::string &form, const std::string &name);
int number(const std::string &s, int low, int high);
Reply api(State &state, const std::string &method, const std::string &path, const std::string &form);
int run(State &state, int port);
}
