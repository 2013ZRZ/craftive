#pragma once

#include <cstddef>
#include <fstream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>
#include <vector>

#include "err.hpp"

using json = nlohmann::json;

class Block {
  private:
    std::string blk;
    std::string id;
    std::string kit;
    std::string name;

  public:
    Block() {
        setBlk("null");
    }
    Block(const unsigned short back) {
        setBlk("null", back);
    }
    Block(const std::string &blk_) {
        setBlk(blk_);
    }
    Block(const std::string &blk_,
          const std::string &id_,
          const std::string &kit_,
          const std::string &name_) {
        setBlk(blk_);
        setID(id_);
        setKit(kit_);
        setName(name_);
    }
    Block(const std::string   &blk_,
          const unsigned short back,
          const std::string   &id_,
          const std::string   &kit_,
          const std::string   &name_) {
        setBlk(blk_, back);
        setID(id_);
        setKit(kit_);
        setName(name_);
    }
    Block(const json &j) {
        fromJson(j);
    }
    Block(const json &j, const std::string &kit_) {
        fromJson(j);
        setKit(kit_);
    }

    std::string getBlk() const & {
        return blk;
    }
    void setBlk(const std::string &blk_) {
        blk = (blk_ == "null" ? "  " : blk_);
    }
    void setBlk(const std::string &blk_, const unsigned short back) {
        if (back > 255) {
            throw errcode(0x0003);
        } else {
            std::ostringstream oss;
            oss << back;
            blk = "\033[48;5;" + oss.str() + "m" + (blk_ == "null" ? "  " : blk_) + "\033[0m";
        }
    }
    void setBack(const unsigned short back) {
        if (back > 255) {
            throw errcode(0x0003);
        } else {
            std::stringstream oss;
            oss << back;
            blk = "\033[48;5;" + oss.str() + "m" + blk + "\033[0m";
        }
    }

    std::string getID() const & {
        return id;
    }
    void setID(const std::string &id_) {
        if (id_.empty())
            throw errcode(0x0001);
        else if (!isValidID(id_))
            throw errcode(0x0002);
        else
            id = id_;
    }

    std::string getKit() const & {
        return kit;
    }
    void setKit(const std::string &kit_) {
        if (kit_.empty())
            throw errcode(0x0001);
        else if (!isValidID(kit_))
            throw errcode(0x0002);
        else
            kit = kit_;
    }

    std::string getName() const & {
        return name;
    }
    void setName(const std::string &name_) {
        name = name_;
    }

    void fromJson(const json &j) {
        setBlk(j.value("blk", "null"));
        setName(j.at("name").template get<std::string>());
        setID(j.value("id", randomID()));
    }
    json toJson() const & {
        json j{{"blk", getBlk()}, {"id", getID()}, {"name", getName()}};
        return j;
    }
}; // class Block

class LargeBlock {
  private:
    std::vector<std::vector<Block>> lblk;
    std::string                     id;
    std::string                     kit;
    std::string                     name;

  public:
    LargeBlock() {}
    LargeBlock(const size_t w, const size_t h) {
        setW(w);
        setH(h);
    }
    LargeBlock(const size_t       w,
               const size_t       h,
               const std::string &id_,
               const std::string &kit_,
               const std::string &name_) {
        setW(w);
        setH(h);
        setID(id_);
        setKit(kit_);
        setName(name_);
    }
    LargeBlock(const size_t                           w,
               const size_t                           h,
               const std::vector<std::vector<Block>> &lblk_,
               const std::string                     &id_,
               const std::string                     &kit_,
               const std::string                     &name_) {
        setW(w);
        setH(h);
        setLblk(lblk_);
        setID(id_);
        setKit(kit_);
        setName(name_);
    }
    LargeBlock(const json &j) {
        fromJson(j);
    }
    LargeBlock(const json &j, const std::string &kit_) {
        fromJson(j);
        setKit(kit_);
    }

    std::vector<std::vector<Block>> getLblk() const & {
        return lblk;
    }
    void setLblk(const std::vector<std::vector<Block>> &lblk_) {
        lblk = lblk_;
    }

    Block getPos(const size_t r, const size_t c) const & {
        if (r >= lblk.size() || c >= lblk[0].size())
            throw errcode(0x0005);
        return lblk[r][c];
    }
    void setPos(const size_t r, const size_t c, const Block &blk) {
        if (r >= lblk.size() || c >= lblk[0].size())
            throw errcode(0x0005);
        lblk[r][c] = blk;
    }

    size_t getW() const {
        return lblk.empty() ? 0 : lblk[0].size();
    }
    void setW(const size_t w) {
        for (auto &row : lblk) {
            row.resize(w);
        }
    }

    size_t getH() const {
        return lblk.size();
    }
    void setH(const size_t h) {
        lblk.resize(h, std::vector<Block>(getW()));
    }

    std::string getID() const & {
        return id;
    }
    void setID(const std::string &id_) {
        if (id_.empty())
            throw errcode(0x0001);
        else if (!isValidID(id_))
            throw errcode(0x0002);
        else
            id = id_;
    }

    std::string getKit() const & {
        return kit;
    }
    void setKit(const std::string &kit_) {
        if (kit_.empty())
            throw errcode(0x0001);
        else if (!isValidID(kit_))
            throw errcode(0x0002);
        else
            kit = kit_;
    }

    std::string getName() const & {
        return name;
    }
    void setName(const std::string &name_) {
        name = name_;
    }

    void fromJson(const json &j) {
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
                setPos(r, c, Block(j.at("blks")[r][c].template get<std::string>()));
            }
        }
    }
    json toJson() const & {
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
}; // class LargeBlock

class Kit {
  private:
    std::string             author;
    std::vector<Block>      blks;
    std::vector<LargeBlock> lblks;
    std::string             id;
    std::string             name;

  public:
    Kit() {}
    Kit(const std::string &path) {
        fromFile(path);
    }

    std::string getAuthor() const & {
        return author;
    }
    void setAuthor(const std::string &author_) {
        author = author_;
    }

    std::vector<Block> getBlks() const & {
        return blks;
    }
    void AddToBlks(const Block &blk) {
        blks.emplace_back(blk);
    }
    void DelFromBlks(const size_t i) {
        if (i < blks.size())
            blks.erase(blks.begin() + i);
        else
            throw errcode(0x0004);
    }
    void ClearBlks() {
        blks.clear();
    }

    std::vector<LargeBlock> getLblks() const & {
        return lblks;
    }
    void AddToLblks(const LargeBlock &lblk) {
        lblks.emplace_back(lblk);
    }
    void DelFromLblks(const size_t i) {
        if (i < lblks.size())
            lblks.erase(lblks.begin() + i);
        else
            throw errcode(0x0004);
    }
    void ClearLblks() {
        lblks.clear();
    }

    std::string getID() const & {
        return id;
    }
    void setID(const std::string &id_) {
        if (id_.empty())
            throw errcode(0x0001);
        else if (!isValidID(id_))
            errcode(0x0002);
        else
            id = id_;
    }

    std::string getName() const & {
        return name;
    }
    void setName(const std::string &name_) {
        name = name_;
    }

    void fromJson(const json &j) {
        setAuthor(j.value("author", "Unknown"));
        setID(j.value("id", randomID()));
        setName(j.value("name", "Empty Name"));
        for (const json &blk : j.at("blks")) {
            AddToBlks(Block(blk, id));
        }
        for (const json &lblk : j.at("lblks")) {
            AddToLblks(LargeBlock(lblk, id));
        }
    }
    json toJson() const & {
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

    void fromFile(const std::string &path) {
        std::ifstream ifs(path);
        if (!ifs)
            throw errcode(strerror(errno));
        json j;
        ifs >> j;
        fromJson(j);
    }
    void toFile(const std::string &path) {
        std::ofstream ofs(path);
        ofs << std::setw(4) << toJson() << std::endl;
    }
}; // class Kit

class Map {
  private:
    std::vector<std::vector<Block>> m;

  public:
    Map() {}
    Map(const size_t w, const size_t h) : m(h, std::vector<Block>(w)) {}
}; // class map
