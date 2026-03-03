#include "elements.hpp"
#include <fstream>

using json = nlohmann::json;


// Definitions in rgb

rgb::rgb() : r(0), g(0), b(0) {}

rgb::rgb(uint8_t _r, uint8_t _g, uint8_t _b) : r(_r), g(_g), b(_b) {}

std::string rgb::toB() const {
    std::ostringstream oss;
    oss << "\033[48;2;" << r << ";" << g << ";" << b << "m";
    return oss.str();
}

std::string rgb::toF() const {
    std::ostringstream oss;
    oss << "\033[38;2;" << r << ";" << g << ";" << b << "m";
    return oss.str();
}


// Definitions in Ucc

Ucc::Ucc(const wchar_t _c) noexcept : c(_c), hasB(false), hasF(false) {}

Ucc::Ucc(const std::string &_str) noexcept : str(_str) {}

Ucc::Ucc(const wchar_t _c, const rgb _b, const rgb _f) noexcept
    : c(_c), hasB(true), b(_b), hasF(true), f(_f) {}

Ucc &Ucc::cb(const wchar_t _c, const rgb _b) noexcept {
    c    = _c;
    hasB = true;
    b    = _b;
    hasF = false;
    return *this;
}

Ucc &Ucc::cf(const wchar_t _c, const rgb _f) noexcept {
    c    = _c;
    hasB = false;
    hasF = true;
    f    = _f;
    return *this;
}

const std::string &Ucc::toStr() const {
    if (str.empty()) {
        std::string _c =
            std::filesystem::path(std::wstring(1, c)).string(); // Convert c(wchar_t) to std::string
        if (hasB) {
            if (hasF) {
                std::ostringstream oss;
                oss << b.toB() << f.toF() << _c << "\033[0m";
                str = oss.str();
            } else {
                std::ostringstream oss;
                oss << b.toB() << _c << "\033[0m";
                str = oss.str();
            }
        } else {
            if (hasF) {
                std::ostringstream oss;
                oss << f.toF() << _c << "\033[0m";
                str = oss.str();
            } else {
                str = _c;
            }
        }
    }
    return str;
}


// Definitions in Element

const std::string &Element::getID() const noexcept { return id; }

void Element::setID(const std::string &_id) {
    if (isInvalidID(_id))
        throw CrtExcept(0x0002, _("from Element::setID()"));
    else
        id = _id;
}

const std::string &Element::getKit() const noexcept { return kit; }

void Element::setKit(const std::string &_kit) {
    if (isInvalidID(_kit))
        throw CrtExcept(0x0002, _("from Element::setKit()"));
    else
        kit = _kit;
}

const std::string &Element::getName() const noexcept { return name; }

void Element::setName(const std::string &_name) noexcept { name = _name; }


// Definitions in Block

Block::Block(const Ucc &_blk) noexcept { setBlk(_blk); }

Block::Block(const Ucc         &_blk,
             const std::string &_id,
             const std::string &_kit,
             const std::string &_name) {
    setBlk(_blk);
    setID(_id);
    setKit(_kit);
    setName(_name);
}

Block::Block(const std::same_as<json> auto &j) { fromJson(j); }

Block::Block(const json &j, const std::string &_kit) {
    fromJson(j);
    setKit(_kit);
}

const std::string &Block::getBlk() const noexcept { return blk.toStr(); }

void Block::setBlk(const Ucc &_blk) noexcept { blk = _blk; }

void Block::fromJson(const json &j) {
    setBlk(Ucc(j.at("blk").template get<std::string>()));
    setID(j.value("id", randomID()));
    setName(j.value("name", id));
}

json Block::toJson() const {
    json j{{"blk", getBlk()}, {"id", getID()}, {"name", getName()}};
    return j;
}


// Definitions in LBlock

LBlock::LBlock(const size_t w, const size_t h) {
    setW(w);
    setH(h);
}

LBlock::LBlock(const size_t       w,
               const size_t       h,
               const std::string &_id,
               const std::string &_kit,
               const std::string &_name) {
    setW(w);
    setH(h);
    setID(_id);
    setKit(_kit);
    setName(_name);
}

LBlock::LBlock(const UCCV2       &_lblk,
               const std::string &_id,
               const std::string &_kit,
               const std::string &_name) {
    setLblk(_lblk);
    setID(_id);
    setKit(_kit);
    setName(_name);
}

LBlock::LBlock(const std::same_as<json> auto &j) { fromJson(j); }

LBlock::LBlock(const json &j, const std::string &_kit) {
    fromJson(j);
    setKit(_kit);
}

void LBlock::setLblk(const UCCV2 &_lblk) { lblk = _lblk; }

std::string LBlock::getLine(const size_t r) const {
    if (r >= lblk.size())
        throw CrtExcept(0x0005, _("from LBlock::getLine()"));
    std::string l;
    for (auto &i : lblk[r]) { l += i.toStr(); }
    return l;
}

const Ucc &LBlock::getPos(const size_t r, const size_t c) const {
    if (r >= lblk.size() || c >= lblk[0].size())
        throw CrtExcept(0x0005, _("from LBlock::getPos()"));
    return lblk[r][c];
}

void LBlock::setPos(const size_t r, const size_t c, const Ucc &blk) {
    if (r >= lblk.size() || c >= lblk[0].size())
        throw CrtExcept(0x0005, _("from LBlock::setPos()"));
    lblk[r][c] = blk;
}

size_t LBlock::getW() const { return w; }

void LBlock::setW(const size_t _w) { w = _w; }

size_t LBlock::getH() const { return h; }

void LBlock::setH(const size_t _h) { h = _h; }

void LBlock::fromJson(const json &j) {
    if (!j.at("blks").is_array())
        throw CrtExcept(
            0x0006, _("from LBlock::fromJson(); the \"blks\" in the json object isn't an array"));
    lblk.resize(j.at("blks").size());
    for (size_t r = 0; r < j.at("blks").size(); r++) {
        if (!j.at("blks")[r].is_array())
            throw CrtExcept(0x0006, _("from LBlock::fromJson(); Row {:z} isn't an array"), r);
        if (j.at("blks")[r].size() != j.at("blks")[0].size())
            throw CrtExcept(
                0x0006,
                _("from LBlock::fromJson(); the length of Row {:z} is different from the others"),
                r);
        for (size_t c = 0; c < j.at("blks")[0].size(); c++) {
            lblk[r].resize(j.at("blks")[r].size());
            setPos(r, c, Ucc(j.at("blks")[r][c].template get<std::string>()));
        }
    }
    setID(j.value("id", randomID()));
    setName(j.value("name", id));
}

json LBlock::toJson() const {
    std::vector<std::vector<std::string>> lblk_json;
    lblk_json.resize(lblk.size());
    for (size_t r = 0; r < lblk.size(); r++) {
        lblk_json[r].resize(lblk.empty() ? 0 : lblk[0].size());
        for (size_t c = 0; c < (lblk.empty() ? 0 : lblk[0].size()); c++) {
            lblk_json[r][c] = lblk[r][c].toStr();
        }
    }
    json j{{"blks", lblk_json}, {"id", getID()}, {"name", getName()}};
    return j;
}


// Definitions in Kit

Kit::Kit(const std::filesystem::path &path) { fromFile(path); }

const std::string &Kit::getAuthor() const noexcept { return author; }

void Kit::setAuthor(const std::string &_author) noexcept { author = _author; }

const BLOCKV &Kit::getBlks() const noexcept { return blks; }

void Kit::clearBlks() noexcept { blks.clear(); }

const LBLOCKV &Kit::getLblks() const noexcept { return lblks; }

void Kit::clearLblks() noexcept { lblks.clear(); }

void Kit::operator+=(const Block &blk) { blks.emplace_back(blk); }

void Kit::operator+=(const LBlock &lblk) { lblks.emplace_back(lblk); }

void Kit::operator-=(const std::string &_id) {
    auto erased_blks = std::erase_if(blks, [&_id](const Block &blk) { return blk.getID() == _id; });
    auto erased_lblks =
        std::erase_if(lblks, [&_id](const LBlock &lblk) { return lblk.getID() == _id; });
    if (erased_blks == 0 && erased_lblks == 0)
        throw CrtExcept(0x0004,
                        _("from Kit::operator-=; no matching ID in both blocks and large-blocks"));
}

const std::string &Kit::getID() const noexcept { return id; }

void Kit::setID(const std::string &_id) {
    if (_id.empty())
        id = randomID();
    else if (isInvalidID(_id))
        CrtExcept(0x0002, _("from Kit::setID()"));
    else
        id = _id;
}

const std::string &Kit::getName() const noexcept { return name; }

void Kit::setName(const std::string &_name) noexcept { name = _name; }

void Kit::fromJson(const json &j) {
    setAuthor(j.value("author", _("Unknown")));
    setID(j.value("id", randomID()));
    setName(j.value("name", _("Empty Name")));
    for (const json &blk : j.at("blks")) { *this += Block(blk, id); }
    for (const json &lblk : j.at("lblks")) { *this += LBlock(lblk, id); }
}

json Kit::toJson() const {
    std::vector<json> blks_json;
    std::vector<json> lblks_json;
    blks_json.reserve(blks.size());
    for (const auto &i : blks) { blks_json.emplace_back(i.toJson()); }
    lblks_json.reserve(lblks.size());
    for (const auto &i : lblks) { lblks_json.emplace_back(i.toJson()); }
    json j{{"author", getAuthor()},
           {"id", getID()},
           {"name", getName()},
           {"blks", blks_json},
           {"lblks", lblks_json}};
    return j;
}

void Kit::fromFile(const std::filesystem::path &path) {
    std::ifstream ifs(path);
    if (!ifs)
        throw CrtExcept(strerror(errno));
    json j;
    ifs >> j;
    fromJson(j);
}
void Kit::toFile(const std::filesystem::path &path, const unsigned tabsize) {
    std::ofstream ofs(path);
    ofs << std::setw(tabsize) << toJson() << std::endl;
}
