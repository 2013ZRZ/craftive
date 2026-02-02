#include <cstddef>
#include <fstream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <vector>

#include "elements.hpp"
#include "err.hpp"

using json = nlohmann::json;

// Definition in Block
Block::Block() {
    setBlk("null");
}
Block::Block(const unsigned short back) {
    setBlk("null", back);
}
Block::Block(const str &blk_) {
    setBlk(blk_);
}
Block::Block(const str &blk_, const str &id_, const str &kit_, const str &name_) {
    setBlk(blk_);
    setID(id_);
    setKit(kit_);
    setName(name_);
}
Block::Block(
    const str &blk_, const unsigned short back, const str &id_, const str &kit_, const str &name_) {
    setBlk(blk_, back);
    setID(id_);
    setKit(kit_);
    setName(name_);
}
Block::Block(const json &j) {
    fromJson(j);
}
Block::Block(const json &j, const str &kit_) {
    fromJson(j);
    setKit(kit_);
}

str Block::getBlk() const & {
    return blk;
}
void Block::setBlk(const str &blk_) {
    blk = (blk_ == "null" ? "  " : blk_);
}
void Block::setBlk(const str &blk_, const unsigned short back) {
    if (back > 255) {
        throw errcode(0x0003);
    } else {
        std::ostringstream oss;
        oss << back;
        blk = "\033[48;5;" + oss.str() + "m" + (blk_ == "null" ? "  " : blk_) + "\033[0m";
    }
}
void Block::setBack(const unsigned short back) {
    if (back > 255) {
        throw errcode(0x0003);
    } else {
        std::stringstream ss;
        ss << back;
        blk = "\033[48;5;" + ss.str() + "m" + blk + "\033[0m";
    }
}

str Block::getID() const & {
    return id;
}
void Block::setID(const str &id_) {
    if (id_.empty())
        throw errcode(0x0001);
    else if (!isValidID(id_))
        throw errcode(0x0002);
    else
        id = id_;
}

str Block::getKit() const & {
    return kit;
}
void Block::setKit(const str &kit_) {
    if (kit_.empty())
        throw errcode(0x0001);
    else if (!isValidID(kit_))
        throw errcode(0x0002);
    else
        kit = kit_;
}

str Block::getName() const & {
    return name;
}
void Block::setName(const str &name_) {
    name = name_;
}

void Block::fromJson(const json &j) {
    setBlk(j.value("blk", "null"));
    setName(j.at("name").template get<str>());
    setID(j.value("id", randomID()));
}
json Block::toJson() const & {
    json j{{"blk", getBlk()}, {"id", getID()}, {"name", getName()}};
    return j;
}

// Definition in LBlock
LBlock::LBlock(const size_t w, const size_t h) {
    setW(w);
    setH(h);
}
LBlock::LBlock(const size_t w, const size_t h, const str &id_, const str &kit_, const str &name_) {
    setW(w);
    setH(h);
    setID(id_);
    setKit(kit_);
    setName(name_);
}
LBlock::LBlock(const size_t  w,
               const size_t  h,
               const BlockT &lblk_,
               const str    &id_,
               const str    &kit_,
               const str    &name_) {
    setW(w);
    setH(h);
    setLblk(lblk_);
    setID(id_);
    setKit(kit_);
    setName(name_);
}
LBlock::LBlock(const json &j) {
    fromJson(j);
}
LBlock::LBlock(const json &j, const str &kit_) {
    fromJson(j);
    setKit(kit_);
}

BlockT LBlock::getLblk() const & {
    return lblk;
}
void LBlock::setLblk(const BlockT &lblk_) {
    lblk = lblk_;
}

Block LBlock::getPos(const size_t r, const size_t c) const & {
    if (r >= lblk.size() || c >= lblk[0].size())
        throw errcode(0x0005);
    return lblk[r][c];
}
void LBlock::setPos(const size_t r, const size_t c, const Block &blk) {
    if (r >= lblk.size() || c >= lblk[0].size())
        throw errcode(0x0005);
    lblk[r][c] = blk;
}

size_t LBlock::getW() const {
    return lblk.empty() ? 0 : lblk[0].size();
}
void LBlock::setW(const size_t w) {
    for (auto &row : lblk) {
        row.resize(w);
    }
}

size_t LBlock::getH() const {
    return lblk.size();
}
void LBlock::setH(const size_t h) {
    lblk.resize(h, BlockV(getW()));
}

void LBlock::fromJson(const json &j) {
    if (!j.at("blks").is_array())
        throw errcode(0x0006);
    setH(j.at("blks").size());
    for (size_t r = 0; r < j.at("blks").size(); r++) {
        if (!j.at("blks")[r].is_array())
            throw errcode(0x0006);
        if (j.at("blks")[r].size() != j.at("blks")[0].size())
            throw errcode(0x0006);
        setW(j.at("blks")[r].size());
        for (size_t c = 0; c < j.at("blks")[0].size(); c++) {
            setPos(r, c, Block(j.at("blks")[r][c].template get<str>()));
        }
    }
}
json LBlock::toJson() const & {
    std::vector<std::vector<json>> lblk_json;
    lblk_json.reserve(getH());
    for (size_t r = 0; r < getH(); r++) {
        lblk_json[r].reserve(getW());
        for (size_t c = 0; c < getW(); c++) {
            lblk_json[r][c] = lblk[r][c].toJson();
        }
    }
    json j{{"blks", lblk_json}, {"id", getID()}, {"name", getName()}};
    return j;
}

// Definition in Kit
Kit::Kit(const str &path) {
    fromFile(path);
}

str Kit::getAuthor() const & {
    return author;
}
void Kit::setAuthor(const str &author_) {
    author = author_;
}

BlockV Kit::getBlks() const & {
    return blks;
}
void Kit::AddToBlks(const Block &blk) {
    blks.emplace_back(blk);
}
void Kit::DelFromBlks(const size_t i) {
    if (i < blks.size())
        blks.erase(blks.begin() + i);
    else
        throw errcode(0x0004);
}
void Kit::ClearBlks() {
    blks.clear();
}

LBlockV Kit::getLblks() const & {
    return lblks;
}
void Kit::AddToLblks(const LBlock &lblk) {
    lblks.emplace_back(lblk);
}
void Kit::DelFromLblks(const size_t i) {
    if (i < lblks.size())
        lblks.erase(lblks.begin() + i);
    else
        throw errcode(0x0004);
}
void Kit::ClearLblks() {
    lblks.clear();
}

str Kit::getID() const & {
    return id;
}
void Kit::setID(const str &id_) {
    if (id_.empty())
        throw errcode(0x0001);
    else if (!isValidID(id_))
        errcode(0x0002);
    else
        id = id_;
}

str Kit::getName() const & {
    return name;
}
void Kit::setName(const str &name_) {
    name = name_;
}

void Kit::fromJson(const json &j) {
    setAuthor(j.value("author", "Unknown"));
    setID(j.value("id", randomID()));
    setName(j.value("name", "Empty Name"));
    for (const json &blk : j.at("blks")) {
        AddToBlks(Block(blk, id));
    }
    for (const json &lblk : j.at("lblks")) {
        AddToLblks(LBlock(lblk, id));
    }
}
json Kit::toJson() const & {
    std::vector<json> blks_json;
    std::vector<json> lblks_json;
    blks_json.reserve(blks.size());
    for (const auto &i : blks) {
        blks_json.emplace_back(i.toJson());
    }
    lblks_json.reserve(lblks.size());
    for (const auto &i : lblks) {
        lblks_json.emplace_back(i.toJson());
    }
    json j{{"author", getAuthor()},
           {"id", getID()},
           {"name", getName()},
           {"blks", blks_json},
           {"lblks", lblks_json}};
    return j;
}

void Kit::fromFile(const str &path) {
    std::ifstream ifs(path);
    if (!ifs)
        throw errcode(strerror(errno));
    json j;
    ifs >> j;
    fromJson(j);
}
void Kit::toFile(const str &path) {
    std::ofstream ofs(path);
    ofs << std::setw(4) << toJson() << std::endl;
}