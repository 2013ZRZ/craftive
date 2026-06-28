#include "status.hpp"

CoreStatus &CoreStatus::instance() {
    static CoreStatus inst;
    return inst;
}