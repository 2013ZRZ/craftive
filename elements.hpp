#pragma once

#include <concepts>
#include <cstddef>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using json = nlohmann::json;

class Block {
  private:
    std::string blk;

  protected:
    std::string id;
    std::string kit;
    std::string name;

  public:
    Block() {}
    Block(const unsigned short back) {}
    Block(const std::string &blk_) {}
    Block(const std::string &blk_,
          const std::string &id_,
          const std::string &kit_,
          const std::string &name_) {}
    Block(const std::string   &blk_,
          const unsigned short back,
          const std::string   &id_,
          const std::string   &kit_,
          const std::string   &name_) {}
    template <typename T>
        requires(!std::convertible_to<T, std::string> && std::convertible_to<T, json>)
    Block(const T &j) {}
    Block(const json &j, const std::string &kit_) {}

    std::string getBlk() const & {}
    void        setBlk(const std::string &blk_) {}
    void        setBlk(const std::string &blk_, const unsigned short back) {}
    void        setBack(const unsigned short back) {}
    std::string getID() const & {}
    void        setID(const std::string &id_) {}
    std::string getKit() const & {}
    void        setKit(const std::string &kit_) {}
    std::string getName() const & {}
    void        setName(const std::string &name_) {}
    void        fromJson(const json &j) {}
    json        toJson() const        &{}
}; // class Block

using BlockV = std::vector<Block>;
using BlockT = std::vector<BlockV>;

class LBlock : public Block {
  private:
    BlockT lblk;

  public:
    LBlock() {}
    LBlock(const size_t w, const size_t h) {}
    LBlock(const size_t       w,
           const size_t       h,
           const std::string &id_,
           const std::string &kit_,
           const std::string &name_) {}
    LBlock(const BlockT      &lblk_,
           const std::string &id_,
           const std::string &kit_,
           const std::string &name_) {}
    template <typename T>
        requires(!std::convertible_to<T, std::string> && std::convertible_to<T, json>)
    LBlock(const T &j) {}
    LBlock(const json &j, const std::string &kit_) {}

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

using LBlockV = std::vector<LBlock>;

class Kit {
  private:
    std::string author;
    BlockV      blks;
    LBlockV     lblks;
    std::string id;
    std::string name;

  public:
    Kit() {}
    Kit(const std::string &path) {}

    std::string getAuthor() const & {}
    void        setAuthor(const std::string &author_) {}
    BlockV      getBlks() const      &{}
    void        AddToBlks(const Block &blk) {}
    void        DelFromBlks(const size_t i) {}
    void        ClearBlks() {}
    LBlockV     getLblks() const     &{}
    void        AddToLblks(const LBlock &lblk) {}
    void        DelFromLblks(const size_t i) {}
    void        ClearLblks() {}
    std::string getID() const & {}
    void        setID(const std::string &id_) {}
    std::string getName() const & {}
    void        setName(const std::string &name_) {}
    void        fromJson(const json &j) {}
    json        toJson() const        &{}
    void        fromFile(const std::string &path) {}
    void        toFile(const std::string &path) {}
}; // class Kit

class Map {
  private:
    BlockT m;

  public:
    Map() {}
    Map(const size_t w, const size_t h) {}
}; // class Map
