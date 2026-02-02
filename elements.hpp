#pragma once

#include <cstddef>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using json = nlohmann::json;
using str  = std::string;

class Block {
  private:
    str blk;

  protected:
    str id;
    str kit;
    str name;

  public:
    Block() {}
    Block(const unsigned short back) {}
    Block(const str &blk_) {}
    Block(const str &blk_, const str &id_, const str &kit_, const str &name_) {}
    Block(const str           &blk_,
          const unsigned short back,
          const str           &id_,
          const str           &kit_,
          const str           &name_) {}
    Block(const json &j) {}
    Block(const json &j, const str &kit_) {}

    str  getBlk() const  &{}
    void setBlk(const str &blk_) {}
    void setBlk(const str &blk_, const unsigned short back) {}
    void setBack(const unsigned short back) {}
    str  getID() const  &{}
    void setID(const str &id_) {}
    str  getKit() const  &{}
    void setKit(const str &kit_) {}
    str  getName() const  &{}
    void setName(const str &name_) {}
    void fromJson(const json &j) {}
    json toJson() const & {}
}; // class Block

using BlockT = std::vector<BlockV>;

class LBlock : public Block {
  private:
    BlockT lblk;

  public:
    LBlock() {}
    LBlock(const size_t w, const size_t h) {}
    LBlock(const size_t w, const size_t h, const str &id_, const str &kit_, const str &name_) {}
    LBlock(const size_t  w,
           const size_t  h,
           const BlockT &lblk_,
           const str    &id_,
           const str    &kit_,
           const str    &name_) {}
    LBlock(const json &j) {}
    LBlock(const json &j, const str &kit_) {}

    BlockT getLblk() const & {}
    void   setLblk(const BlockT &lblk_) {}
    Block  getPos(const size_t r, const size_t c) const  &{}
    void   setPos(const size_t r, const size_t c, const Block &blk) {}
    size_t getW() const {}
    void   setW(const size_t w) {}
    size_t getH() const {}
    void   setH(const size_t h) {}
    void   fromJson(const json &j) {}
    json   toJson() const   &{}
}; // class LBlock

using BlockV  = std::vector<Block>;
using LBlockV = std::vector<LBlock>;

class Kit {
  private:
    str     author;
    BlockV  blks;
    LBlockV lblks;
    str     id;
    str     name;

  public:
    Kit() {}
    Kit(const str &path) {}

    str     getAuthor() const     &{}
    void    setAuthor(const str &author_) {}
    BlockV  getBlks() const  &{}
    void    AddToBlks(const Block &blk) {}
    void    DelFromBlks(const size_t i) {}
    void    ClearBlks() {}
    LBlockV getLblks() const & {}
    void    AddToLblks(const LBlock &lblk) {}
    void    DelFromLblks(const size_t i) {}
    void    ClearLblks() {}
    str     getID() const     &{}
    void    setID(const str &id_) {}
    str     getName() const     &{}
    void    setName(const str &name_) {}
    void    fromJson(const json &j) {}
    json    toJson() const    &{}
    void    fromFile(const str &path) {}
    void    toFile(const str &path) {}
}; // class Kit

class Map {
  private:
    BlockT m;

  public:
    Map() {}
    Map(const size_t w, const size_t h) {}
}; // class Map
