#include "crtutils.hpp"
#include "err.hpp"
#include <bit>
#include <ctime>
#include <iconv.h>
#include <random>

bool isInvalidID(const std::string &id) {
    for (const auto it : id) {
        if (!isalnum(it) && it != '_')
            return true;
    }
    return false;
}

std::string randomID(unsigned len) {
    std::random_device rd;
    std::string        o;
    std::mt19937_64    r(time(nullptr) + rd());
    for (unsigned i = 0; i < len; i++) {
        uint8_t _r = r() % 63;
        if (_r < 26)
            o.push_back((char)'a' + _r); // 0 ~ 25 : 'a' ~ 'z'
        else if (_r < 52)
            o.push_back((char)'A' + _r - 26); // 26 ~ 51 : 'A' ~ 'Z'
        else if (_r < 62)
            o.push_back((char)'0' + _r - 52); // 52 ~ 61 : '0' ~ '9'
        else
            o.push_back('_'); // 62 : '_'
    }
    return o;
}

namespace UtfConverterDetail {
static const char *getU32Encoding() {
    if constexpr (std::endian::native == std::endian::little) {
        return "UTF-32LE";
    } else {
        return "UTF-32BE";
    }
}
} // namespace UtfConverterDetail

std::u32string s2u32s(const std::string &s) {
    if (s.empty())
        return std::u32string();

    iconv_t cd = iconv_open(UtfConverterDetail::getU32Encoding(), "UTF-8");
    if (cd == (iconv_t)(-1))
        throw CrtExcept("from s2u32s(); failed to open iconv for UTF-8 to UTF-32 ({})",
                        strerror(errno));

    size_t inbytesleft = s.size();
    char  *inbuf       = const_cast<char *>(s.data());

    size_t      outbytesleft = (s.size() + 3) * sizeof(char32_t);
    std::string u32_buffer(outbytesleft, '\0');
    char       *outbuf          = u32_buffer.data();
    char       *original_outbuf = outbuf;

    size_t result = iconv(cd, &inbuf, &inbytesleft, &outbuf, &outbytesleft);
    iconv_close(cd);

    if (result == (size_t)(-1))
        throw CrtExcept("from s2u32s(); conversion from UTF-8 to UTF-32 failed ({})",
                        strerror(errno));

    size_t u32_char_count = (outbuf - original_outbuf) / sizeof(char32_t);
    return std::u32string(reinterpret_cast<const char32_t *>(original_outbuf), u32_char_count);
}

std::string u32s2s(const std::u32string &s) {
    if (s.empty())
        return std::string();

    iconv_t cd = iconv_open("UTF-8", UtfConverterDetail::getU32Encoding());
    if (cd == (iconv_t)(-1))
        throw CrtExcept("from u32s2s(); failed to open iconv for UTF-32 to UTF-8 ({})",
                        strerror(errno));

    size_t inbytesleft = s.size() * sizeof(char32_t);
    char  *inbuf       = const_cast<char *>(reinterpret_cast<const char *>(s.data()));

    size_t      outbytesleft = s.size() * 4;
    std::string utf8_buffer(outbytesleft, '\0');
    char       *outbuf          = utf8_buffer.data();
    char       *original_outbuf = outbuf;

    size_t result = iconv(cd, &inbuf, &inbytesleft, &outbuf, &outbytesleft);
    iconv_close(cd);

    if (result == (size_t)(-1))
        throw CrtExcept("from u32s2s(); conversion from UTF-32 to UTF-8 failed ({})",
                        strerror(errno));

    return std::string(original_outbuf, outbuf - original_outbuf);
}