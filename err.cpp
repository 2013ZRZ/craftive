#include "err.hpp"
#include <cctype>
#include <ctime>
#include <iomanip>
#include <random>
#include <sstream>

bool isValidID(const std::string &id) {
    for (const auto it : id) {
        if (!isalnum(it) && it != '_')
            return false;
    }
    return true;
}

std::string randomID(unsigned len) {
    std::random_device rd;
    std::string        o;
    std::mt19937_64    r(time(nullptr) + rd());
    for (unsigned i = 0; i < len; i++) {
        unsigned short _r = r() % 26;
        if (_r < 25) {
            o.push_back((char)'a' + _r);
        } else {
            o.push_back('_');
        }
    }
    return o;
}

CrtExcept::CrtExcept(const unsigned short _code, const char *_detail)
    : code(_code), detail(_detail) {}

CrtExcept::CrtExcept(const char *_detail) : CrtExcept(0x0000, _detail) {}

const char *CrtExcept::how() const noexcept {
    return detail;
}

const char *CrtExcept::what() const noexcept {
    return _(errmsgs[code]);
}

unsigned short CrtExcept::which() const noexcept {
    return code;
}

std::string CrtExcept::whichStr() const {
    std::ostringstream oss;
    oss << "0x" << std::uppercase << std::hex << std::setw(4) << std::setfill('0') << code;
    return oss.str();
}
