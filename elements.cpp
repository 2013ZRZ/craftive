#include "elements.hpp"
#include "err.hpp"
#include <fstream>


// Definitions in rgb

rgb::rgb() noexcept : r(0), g(0), b(0) {}

rgb::rgb(uint8_t _r, uint8_t _g, uint8_t _b) noexcept : r(_r), g(_g), b(_b) {}

void rgb::fromJson(const json &j) {
    if (!j.is_array())
        throw CrtExcept(0x0008,
                        _("from rgb::fromJson(); the RGB color in the json object isn't an array"));
    if (j.size() != 3)
        throw CrtExcept(0x0008, _("from rgb::fromJson(); incorrect number of color channels"));
    r = j[0].get<uint8_t>();
    g = j[1].get<uint8_t>();
    b = j[2].get<uint8_t>();
}

json rgb::toJson() const { return json{r, g, b}; }

// Definitions in Ucc

Ucc::Ucc(const char32_t _c) noexcept : c(_c), hasB(false), hasF(false) {}

Ucc::Ucc(const char32_t _c, const rgb _b, const rgb _f) noexcept
    : c(_c), hasB(true), b(_b), hasF(true), f(_f) {}

Ucc::Ucc(const is_json auto &j) { fromJson(j); }

Ucc &Ucc::cb(const char32_t _c, const rgb _b) noexcept {
    c    = _c;
    hasB = true;
    b    = _b;
    hasF = false;
    return *this;
}

Ucc &Ucc::cf(const char32_t _c, const rgb _f) noexcept {
    c    = _c;
    hasB = false;
    hasF = true;
    f    = _f;
    return *this;
}

void Ucc::fromJson(const json &j) {
    if (!j.is_object())
        throw CrtExcept(
            0x0006,
            _("from Ucc::fromJson(); the unicode colored character's json isn't an object"));
    std::string _c = j.at("c").get<std::string>();
    if (_c.size() > sizeof(char32_t))
        throw CrtExcept(
            0x0003,
            "from Ucc::fromJson(); the string to parse is {} and it has too many characters",
            _c);
    c = s2u32s(_c)[0];
    if (j.find("b") != j.end()) {
        hasB = true;
        b.fromJson(j["b"]);
    }
    if (j.find("f") != j.end()) {
        hasF = true;
        f.fromJson(j["f"]);
    }
}

json Ucc::toJson() const {
    json j;
    j["c"] = u32s2s(std::u32string(1, c));
    if (hasB)
        j["b"] = b.toJson();
    if (hasF)
        j["f"] = f.toJson();
    return j;
}

std::string Ucc::operator()() const { return u32s2s(std::u32string(1, c)); }


// Definitions in Element

const std::string &Element::getID() const noexcept { return id; }

void Element::setID(const std::string &_id) {
    if (isInvalidID(_id))
        throw CrtExcept(0x0002, _("from Element::setID()"));
    else
        id = _id;
}

const std::string &Element::getName() const noexcept { return name; }

void Element::setName(const std::string &_name) noexcept { name = _name; }

bool Element::operator==(const Element &other) const noexcept { return id == other.getID(); }


// Definitions in Block

Block::Block(const Ucc &_blk) noexcept { setBlk(_blk); }

Block::Block(const Ucc &_blk, const std::string &_id, const std::string &_name) {
    setBlk(_blk);
    setID(_id);
    setName(_name);
}

Block::Block(const is_json auto &j) { fromJson(j); }

const Ucc &Block::getBlk() const noexcept { return blk; }

void Block::setBlk(const Ucc &_blk) noexcept { blk = _blk; }

void Block::fromJson(const json &j) {
    setBlk(Ucc(j.at("blk")));
    setID(j.value("id", randomID()));
    setName(j.value("name", id));
}

json Block::toJson() const {
    json j{{"blk", getBlk().toJson()}, {"id", getID()}, {"name", getName()}};
    return j;
}


// Definitions in LBlock

LBlock::LBlock(const size_t _w, const size_t _h) {
    setW(_w);
    setH(_h);
}

LBlock::LBlock(const size_t _w, const size_t _h, const std::string &_id, const std::string &_name) {
    setW(_w);
    setH(_h);
    setID(_id);
    setName(_name);
}

LBlock::LBlock(const UccV2 &_lblk, const std::string &_id, const std::string &_name) {
    setLblk(_lblk);
    setW(_lblk.empty() ? 0 : _lblk[0].size());
    setH(_lblk.size());
    setID(_id);
    setName(_name);
}

LBlock::LBlock(const is_json auto &j) { fromJson(j); }

const UccV2 &LBlock::getLblk() const noexcept { return lblk; }

void LBlock::setLblk(const UccV2 &_lblk) {
    if (!_lblk.empty())
        for (size_t r = 0; r < _lblk.size(); r++)
            if (_lblk[r].size() != _lblk[0].size())
                throw CrtExcept(
                    0x0007, _("from LBlock::setLblk(); the length of Row {} is different"), r + 1);
    lblk = _lblk;
}

const Ucc &LBlock::getPos(const size_t r, const size_t c) const {
    if (r >= lblk.size() || c >= (lblk.empty() ? 0 : lblk[0].size()))
        throw CrtExcept(0x0005, _("from LBlock::getPos()"));
    return lblk[r][c];
}

void LBlock::setPos(const size_t r, const size_t c, const Ucc &blk) {
    if (r >= lblk.size() || c >= (lblk.empty() ? 0 : lblk[0].size()))
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
            throw CrtExcept(0x0006, _("from LBlock::fromJson(); Row {} isn't an array"), r + 1);
        if (j.at("blks")[r].size() != j.at("blks")[0].size())
            throw CrtExcept(
                0x0006, _("from LBlock::fromJson(); the length of Row {} is different"), r + 1);
        for (size_t c = 0; c < j.at("blks")[0].size(); c++) {
            lblk[r].resize(j.at("blks")[r].size());
            setPos(r, c, Ucc(j.at("blks")[r][c]));
        }
    }
    setW(j.value("w", lblk.empty() ? 0 : lblk[0].size()));
    setH(j.value("h", lblk.size()));
    setID(j.value("id", randomID()));
    setName(j.value("name", id));
}

json LBlock::toJson() const {
    std::vector<std::vector<json>> lblk_json;
    lblk_json.resize(lblk.size());
    for (size_t r = 0; r < lblk.size(); r++) {
        lblk_json[r].resize(lblk.empty() ? 0 : lblk[0].size());
        for (size_t c = 0; c < (lblk.empty() ? 0 : lblk[0].size()); c++) {
            lblk_json[r][c] = lblk[r][c].toJson();
        }
    }
    json j{{"blks", lblk_json}, {"w", getW()}, {"h", getH()}, {"id", getID()}, {"name", getName()}};
    return j;
}


// Definitions in Kit

Kit::Kit(const std::filesystem::path &path) { fromFile(path); }

const std::string &Kit::getAuthor() const noexcept { return author; }

void Kit::setAuthor(const std::string &_author) noexcept { author = _author; }

const BlockV &Kit::getBlks() const noexcept { return blks; }

void Kit::clearBlks() noexcept { blks.clear(); }

const LBlockV &Kit::getLblks() const noexcept { return lblks; }

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

const std::string &Kit::getDes() const noexcept { return des; }

void Kit::setDes(const std::string &_des) noexcept { des = _des; }

void Kit::fromJson(const json &j) {
    setAuthor(j.value("author", _("Unknown")));
    setID(j.value("id", randomID()));
    setName(j.value("name", _("Empty Name")));
    setDes(j.value("des", _("Empty")));
    if (j.find("blks") != j.end())
        for (const json &blk : j["blks"]) *this += Block(blk);
    if (j.find("lblks") != j.end())
        for (const json &lblk : j["lblks"]) *this += LBlock(lblk);
}

json Kit::toJson() const {
    if (blks.empty()) {
        if (lblks.empty())
            return json{
                {"author", getAuthor()}, {"id", getID()}, {"name", getName()}, {"des", getDes()}};
        else {
            std::vector<json> lblks_json;
            lblks_json.reserve(lblks.size());
            for (const auto &i : lblks) lblks_json.emplace_back(i.toJson());
            return json{{"author", getAuthor()},
                        {"id", getID()},
                        {"name", getName()},
                        {"des", getDes()},
                        {"lblks", lblks_json}};
        }
    } else {
        std::vector<json> blks_json;
        blks_json.reserve(blks.size());
        for (const auto &i : blks) blks_json.emplace_back(i.toJson());
        if (lblks.empty())
            return json{{"author", getAuthor()},
                        {"id", getID()},
                        {"name", getName()},
                        {"des", getDes()},
                        {"blks", blks_json}};
        else {
            std::vector<json> lblks_json;
            lblks_json.reserve(lblks.size());
            for (const auto &i : lblks) lblks_json.emplace_back(i.toJson());
            return json{{"author", getAuthor()},
                        {"id", getID()},
                        {"name", getName()},
                        {"des", getDes()},
                        {"blks", blks_json},
                        {"lblks", lblks_json}};
        }
    }
}

void Kit::fromFile(const std::filesystem::path &path) {
    std::ifstream ifs(path);
    if (!ifs)
        throw CrtExcept("from Kit::fromFile(); failed to open the kit file at {} ({})",
                        path.string(),
                        strerror(errno));
    json j;
    ifs >> j;
    fromJson(j);
}
void Kit::toFile(const std::filesystem::path &path, const unsigned tabsize) {
    std::ofstream ofs(path);
    ofs << std::setw(tabsize) << toJson() << std::endl;
}
