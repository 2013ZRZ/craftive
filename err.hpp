#pragma once

#include <stdexcept>
#include <string>
#include <unordered_map>

bool        isValidID(const std::string &id);
std::string randomID();

const std::unordered_map<unsigned short, std::string> errtable{
    {0x0001, "0x0001 Empty ID"},
    {0x0002, "0x0002 The ID includes invalid character(s)"},
    {0x0003, "0x0003 Invalid color code; type `help --color-code` for more info"},
    {0x0004, "0x0004 No such element in this kit"},
    {0x0005, "0x0005 Invalid position"},
    {0x0006, "0x0006 Failed to parse JSON"}};
class errcode : public std::runtime_error {
  public:
    errcode(const unsigned short code) : std::runtime_error(errtable.at(code)) {}
    errcode(const std::string &msg) : std::runtime_error("0x0000 Extern Error: " + msg) {}
};