#pragma once

#include <exception>
#include <string>
#include <unordered_map>

#define UNKNOWNERR "0xFFFF Unknown error"

bool        isValidID(const std::string &id);
std::string randomID();

const std::unordered_map<unsigned short, std::string> errtable{
    {0x0001, "Empty ID"},
    {0x0002, "The ID includes invalid character(s)"},
    {0x0003, "Invalid color code; type `help --color-code` for more info"},
    {0x0004, "No such element in this kit"},
    {0x0005, "Invalid position"},
    {0x0006, "Failed to parse JSON"}};

// Craftive Exceptions
class CrtExcept : public std::exception {
  private:
    std::string msg;

  public:
    CrtExcept(const unsigned short code);
    CrtExcept(const std::string &_msg);

    const char *what() const noexcept;
};