#include "elements.hpp"
#include "crtutils.hpp"
#include "err.hpp"
#include <fstream>
#include <variant>


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


// Definitions in BasicCrtClass

const std::string &BasicCrtClass::getID() const noexcept { return id; }

void BasicCrtClass::setID(const std::string &_id) {
    if (isInvalidID(_id))
        throw CrtExcept(0x0002, _("from BasicCrtClass::setID()"));
    else if (_id.empty())
        throw CrtExcept(0x0001, _("from BasicCrtClass::setID()"));
    else
        id = _id;
}

const std::string &BasicCrtClass::getName() const noexcept { return name; }

void BasicCrtClass::setName(const std::string &_name) noexcept { name = _name; }

bool BasicCrtClass::operator==(const BasicCrtClass &other) const noexcept {
    return id == other.getID();
}


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
    if (j.find("id") != j.end())
        setID(j.at("id").get<std::string>());
    else
        throw CrtExcept(0x0001, _("from Block::fromJson()"));
    setName(j.value("name", id));
}

json Block::toJson() const {
    json j{{"blk", getBlk().toJson()}, {"id", getID()}, {"name", getName()}};
    return j;
}


// Definitions in LBlock

LBlock::LBlock(const size_t _w, const size_t _h) {
    lblk.resize(_h);
    w.resize(_h);
    for (auto &i : lblk) i.resize(_w);
}

LBlock::LBlock(const size_t _w, const size_t _h, const std::string &_id, const std::string &_name) {
    lblk.resize(_h);
    w.resize(_h);
    for (auto &i : lblk) i.resize(_w);
    setID(_id);
    setName(_name);
}

LBlock::LBlock(const UccV2 &_lblk, const std::string &_id, const std::string &_name) {
    setLblk(_lblk);
    setID(_id);
    setName(_name);
}

LBlock::LBlock(const is_json auto &j) { fromJson(j); }

const UccV2 &LBlock::getLblk() const noexcept { return lblk; }

void LBlock::setLblk(const UccV2 &_lblk) {
    lblk = _lblk;
    w.resize(lblk.size());
    for (size_t i = 0; i < w.size(); i++) w[i] = lblk[i].size();
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

size_t LBlock::getW(const size_t r) const {
    if (r < w.size())
        return w[r];
    else
        throw CrtExcept(0x0005, _("from LBlock::getW()"));
}

auto LBlock::getFullW() const noexcept -> const std::vector<size_t> & { return w; }

void LBlock::setW(const size_t r, const size_t _w) {
    if (r < w.size())
        w[r] = _w;
    else
        throw CrtExcept(0x0005, _("from LBlock::setW()"));
}

void LBlock::fromJson(const json &j) {
    if (!j.at("lblk").is_array())
        throw CrtExcept(0x0006, _("from LBlock::fromJson(); the \"lblk\" isn't an array"));
    lblk.resize(j.at("lblk").size());
    w.resize(j.at("lblk").size());
    for (size_t r = 0; r < j.at("lblk").size(); r++) {
        if (!j.at("lblk")[r].is_array())
            throw CrtExcept(0x0006, _("from LBlock::fromJson(); Row {} isn't an array"), r + 1);
        for (size_t c = 0; c < j.at("lblk")[0].size(); c++) {
            lblk[r].resize(j.at("lblk")[r].size());
            setPos(r, c, Ucc(j.at("lblk")[r][c]));
        }
    }
    if (!j.at("w").is_array())
        throw CrtExcept(0x0006, _("from LBlock::fromJson(); the \"w\" isn't an array"));
    for (size_t i = 0; i < j.at("w").size(); i++) w[i] = j.at("w")[i].get<size_t>();
    if (j.find("id") != j.end())
        setID(j.at("id").get<std::string>());
    else
        throw CrtExcept(0x0001, _("from LBlock::fromJson()"));
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
    return json{{"lblk", lblk_json}, {"w", getFullW()}, {"id", getID()}, {"name", getName()}};
}

// Definitions in BasicProduct

const std::string &BasicProduct::getAuthor() const noexcept { return author; }

void BasicProduct::setAuthor(const std::string &_author) {
    if (isInvalidEmail(_author))
        throw CrtExcept(
            0x0007,
            _("from BasicProduct::setAuthor(); the string is \"{}\" and it isn't a valid "
              "email address"),
            _author);
    else
        author = _author;
}

const std::string &BasicProduct::getDes() const noexcept { return des; }

void BasicProduct::setDes(const std::string &_des) noexcept { des = _des; }

uint32_t BasicProduct::getPrice() const noexcept { return price; }

void BasicProduct::setPrice(uint32_t _price) noexcept { price = _price; }


// Definitions in Kit

Kit::Kit(const std::filesystem::path &path) { fromFile(path); }

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
        throw CrtExcept(
            0x0004,
            _("from Kit::operator-=; no one's id is \"{}\" in both blocks and large-blocks"),
            _id);
}

auto Kit::operator[](const std::string &_id) -> const std::variant<Block, LBlock> {
    for (const auto &i : blks)
        if (i.getID() == _id)
            return i;
    for (const auto &i : lblks)
        if (i.getID() == _id)
            return i;
    throw CrtExcept(
        0x0004,
        _("from Kit::operator[]; no one's id is \"{}\" in both blocks and large-blocks"),
        _id);
}

void Kit::fromJson(const json &j) {
    setAuthor(j.value("author", _("Unknown")));
    if (j.find("id") != j.end())
        setID(j.at("id").get<std::string>());
    else
        throw CrtExcept(0x0001, _("from Block::fromJson()"));
    setName(j.value("name", id));
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