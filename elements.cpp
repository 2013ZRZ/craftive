#include "elements.hpp"
#include <fstream>
#include <sstream>

using json = nlohmann::json;

// Definitions in Block

Block::Block() noexcept { setBlk("null"); }
Block::Block(const unsigned short back) { setBlk("null", back); }
Block::Block(const std::string &_blk) noexcept { setBlk(_blk); }
Block::Block(const std::string &_blk, const unsigned short back) { setBlk(_blk, back); }
Block::Block(const std::string &_blk,
             const std::string &_id,
             const std::string &_kit,
             const std::string &_name) {
    setBlk(_blk);
    setID(_id);
    setKit(_kit);
    setName(_name);
}
Block::Block(const std::string   &_blk,
             const unsigned short back,
             const std::string   &_id,
             const std::string   &_kit,
             const std::string   &_name) {
    setBlk(_blk, back);
    setID(_id);
    setKit(_kit);
    setName(_name);
}
Block::Block(const std::same_as<json> auto &j) { fromJson(j); }
Block::Block(const json &j, const std::string &_kit) {
    fromJson(j);
    setKit(_kit);
}

std::string Block::getBlk() const & noexcept { return blk.empty() ? "  " : blk; }
void Block::setBlk(const std::string &_blk) noexcept { blk = (_blk == "null" ? "  " : _blk); }
void Block::setBlk(const std::string &_blk, const unsigned short back) {
    if (back > 255) {
        throw CrtExcept(0x0003, _("from Block::setBlk()"));
    } else {
        std::ostringstream oss;
        oss << back;
        blk = "\033[48;5;" + oss.str() + "m" + (_blk == "null" ? "  " : _blk) + "\033[0m";
    }
}
void Block::setBack(const unsigned short back) {
    if (back > 255) {
        throw CrtExcept(0x0003, _("from Block::setBack()"));
    } else {
        std::ostringstream oss;
        oss << back;
        blk = "\033[48;5;" + oss.str() + "m" + blk + "\033[0m";
    }
}

std::string Block::getID() const & noexcept { return id; }
void        Block::setID(const std::string &_id) {
    if (_id.empty())
        id = randomID();
    else if (isInvalidID(_id))
        throw CrtExcept(0x0002, _("from Block::setID()"));
    else
        id = _id;
}

std::string Block::getKit() const & noexcept { return kit; }
void        Block::setKit(const std::string &_kit) {
    if (_kit.empty())
        kit = randomID();
    else if (isInvalidID(_kit))
        throw CrtExcept(0x0002, _("from Block::setKit()"));
    else
        kit = _kit;
}

std::string Block::getName() const & noexcept { return name; }
void        Block::setName(const std::string &_name) noexcept { name = _name; }

void Block::fromJson(const json &j) {
    setBlk(j.value("blk", "null"));
    setID(j.value("id", randomID()));
    setName(j.value("name", id));
}
json Block::toJson() const & {
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
LBlock::LBlock(const std::vector<std::vector<std::string>> &_lblk,
               const std::string                           &_id,
               const std::string                           &_kit,
               const std::string                           &_name) {
    setW(_lblk.empty() ? 0 : _lblk[0].size());
    setH(_lblk.size());
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

void LBlock::setLblk(const std::vector<std::vector<std::string>> &_lblk) {
    setH(_lblk.size());
    setW(_lblk.empty() ? 0 : _lblk[0].size());
    lblk = _lblk;
}

std::string LBlock::getLine(const size_t r) const & {
    if (r >= lblk.size())
        throw CrtExcept(0x0005, _("from LBlock::getLine()"));
    std::string l;
    for (auto &i : lblk[r]) { l += i; }
    return l;
}

std::string LBlock::getPos(const size_t r, const size_t c) const & {
    if (r >= lblk.size() || c >= lblk[0].size())
        throw CrtExcept(0x0005, _("from LBlock::getPos()"));
    return lblk[r][c];
}
void LBlock::setPos(const size_t r, const size_t c, const std::string &blk) {
    if (r >= lblk.size() || c >= lblk[0].size())
        throw CrtExcept(0x0005, _("from LBlock::setPos()"));
    lblk[r][c] = blk;
}

size_t LBlock::getW() const noexcept { return lblk.empty() ? 0 : lblk[0].size(); }
void   LBlock::setW(const size_t w) {
    for (auto &row : lblk) { row.resize(w); }
}

size_t LBlock::getH() const noexcept { return lblk.size(); }
void   LBlock::setH(const size_t h) { lblk.resize(h); }

void LBlock::fromJson(const json &j) {
    if (!j.at("blks").is_array())
        throw CrtExcept(0x0006, _("from LBlock::fromJson()"));
    setH(j.at("blks").size());
    for (size_t r = 0; r < j.at("blks").size(); r++) {
        if (!j.at("blks")[r].is_array())
            throw CrtExcept(0x0006, _("from LBlock::fromJson()"));
        if (j.at("blks")[r].size() != j.at("blks")[0].size())
            throw CrtExcept(0x0006, _("from LBlock::fromJson()"));
        setW(j.at("blks")[r].size());
        for (size_t c = 0; c < j.at("blks")[0].size(); c++) {
            setPos(r, c, j.at("blks")[r][c].template get<std::string>());
        }
    }
    setID(j.value("id", randomID()));
    setName(j.value("name", id));
}
json LBlock::toJson() const & {
    std::vector<std::vector<std::string>> lblk_json;
    lblk_json.resize(getH());
    for (size_t r = 0; r < getH(); r++) {
        lblk_json[r].resize(getW());
        for (size_t c = 0; c < getW(); c++) { lblk_json[r][c] = lblk[r][c]; }
    }
    json j{{"blks", lblk_json}, {"id", getID()}, {"name", getName()}};
    return j;
}

// Definitions in Kit

Kit::Kit(const std::string &path) { fromFile(path); }

std::string Kit::getAuthor() const & noexcept { return author; }
void        Kit::setAuthor(const std::string &_author) noexcept { author = _author; }

std::vector<Block> Kit::getBlks() const & noexcept { return blks; }
void               Kit::AddToBlks(const Block &blk) { blks.emplace_back(blk); }
void               Kit::DelFromBlks(const size_t i) {
    if (i < blks.size())
        blks.erase(blks.begin() + i);
    else
        throw CrtExcept(0x0004, _("from Kit::DelFromBlks()"));
}
void Kit::ClearBlks() noexcept { blks.clear(); }

std::vector<LBlock> Kit::getLblks() const & noexcept { return lblks; }
void                Kit::AddToLblks(const LBlock &lblk) { lblks.emplace_back(lblk); }
void                Kit::DelFromLblks(const size_t i) {
    if (i < lblks.size())
        lblks.erase(lblks.begin() + i);
    else
        throw CrtExcept(0x0004, _("from Kit::DelFromLBlks()"));
}
void Kit::ClearLblks() noexcept { lblks.clear(); }

std::string Kit::getID() const & noexcept { return id; }
void        Kit::setID(const std::string &_id) {
    if (_id.empty())
        id = randomID();
    else if (isInvalidID(_id))
        CrtExcept(0x0002, _("from Kit::setID()"));
    else
        id = _id;
}

std::string Kit::getName() const & noexcept { return name; }
void        Kit::setName(const std::string &_name) noexcept { name = _name; }

void Kit::fromJson(const json &j) {
    setAuthor(j.value("author", "Unknown"));
    setID(j.value("id", randomID()));
    setName(j.value("name", "Empty Name"));
    for (const json &blk : j.at("blks")) { AddToBlks(Block(blk, id)); }
    for (const json &lblk : j.at("lblks")) { AddToLblks(LBlock(lblk, id)); }
}
json Kit::toJson() const & {
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

void Kit::fromFile(const std::string &path) {
    std::ifstream ifs(path);
    if (!ifs)
        throw CrtExcept(strerror(errno));
    json j;
    ifs >> j;
    fromJson(j);
}
void Kit::toFile(const std::string &path, const unsigned tabsize) {
    std::ofstream ofs(path);
    ofs << std::setw(tabsize) << toJson() << std::endl;
}
