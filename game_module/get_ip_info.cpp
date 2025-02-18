#include <memory>
#include <string>
#include "../tutil/tc_file.h"
#include "../third/geo2ip/GeoLite2PP.hpp"

std::string getDBFile() {
    return "../third/geo2ip/data.mmdb";
}

std::string getInfo(const std::string &ip) {
    auto dbFile = getDBFile();
    if (!tars::TC_File::isFileExist(dbFile)) {
        std::cout << "文件不存在" << std::endl;
        return "error";
    }
    auto ptrGeo = make_shared<GeoLite2PP::DB>(dbFile);
    if (nullptr == ptrGeo) {
        std::cout << "构建DB错误" << std::endl;
        return "error";
    }
    try {
        auto result = ptrGeo->lookup(ip);
        std::cout << result << std::endl;
        // return ptrGeo->get_field(&result, "", GeoLite2PP::VCStr{"country", "iso_code"});
    } catch (...) {
        std::cout << "search ip code error!" << ip << std::endl;
        return "error";
    }
}

std::string getIpCode(const std::string &ip) {
    if (ip.empty()) {
        return "error";
    }
    auto code = getInfo(ip);
    std::cout << code << std::endl;
}

int main() {
    getIpCode("113.89.235.57");
}