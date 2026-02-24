#pragma once

#include "err.hpp"
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
    Block() noexcept;
    explicit Block(const unsigned short back);
    explicit Block(const std::string &_blk) noexcept;
    Block(const std::string &_blk, const unsigned short back);
    Block(const std::string &_blk,
          const std::string &_id,
          const std::string &_kit,
          const std::string &_name);
    Block(const std::string   &_blk,
          const unsigned short back,
          const std::string   &_id,
          const std::string   &_kit,
          const std::string   &_name);
    explicit Block(const std::same_as<json> auto &j);
    Block(const json &j, const std::string &_kit);

    std::string getBlk() const & noexcept;
    void        setBlk(const std::string &_blk) noexcept;
    void        setBlk(const std::string &_blk, const unsigned short back);
    void        setBack(const unsigned short back);
    std::string getID() const & noexcept;
    void        setID(const std::string &_id = randomID());
    std::string getKit() const & noexcept;
    void        setKit(const std::string &_kit = randomID());
    std::string getName() const & noexcept;
    void        setName(const std::string &_name) noexcept;
    void        fromJson(const json &j);
    json        toJson() const &;
}; // class Block

// Large block (a rectangular combination of blocks).
class LBlock : public Block {
  private:
    std::vector<std::vector<Block>> lblk;

  public:
    LBlock() {}
    LBlock(const size_t w, const size_t h);
    LBlock(const size_t       w,
           const size_t       h,
           const std::string &_id,
           const std::string &_kit,
           const std::string &_name);
    LBlock(const std::vector<std::vector<Block>> &_lblk,
           const std::string                     &_id,
           const std::string                     &_kit,
           const std::string                     &_name);
    explicit LBlock(const std::same_as<json> auto &j);
    LBlock(const json &j, const std::string &_kit);

    std::vector<std::vector<Block>> getLblk() const & noexcept;
    void                            setLblk(const std::vector<std::vector<Block>> &_lblk) noexcept;
    Block                           getPos(const size_t r, const size_t c) const &;
    void                            setPos(const size_t r, const size_t c, const Block &blk);
    size_t                          getW() const noexcept;
    void                            setW(const size_t w);
    size_t                          getH() const noexcept;
    void                            setH(const size_t h);
    void                            fromJson(const json &j);
    json                            toJson() const &;
}; // class LBlock

// Where stores data of blocks and large-blocks.
class Kit {
  private:
    std::string         author;
    std::vector<Block>  blks;
    std::vector<LBlock> lblks;
    std::string         id;
    std::string         name;

  public:
    Kit() = default;
    explicit Kit(const std::string &path);

    std::string         getAuthor() const & noexcept;
    void                setAuthor(const std::string &_author) noexcept;
    std::vector<Block>  getBlks() const & noexcept;
    void                AddToBlks(const Block &blk);
    void                DelFromBlks(const size_t i);
    void                ClearBlks() noexcept;
    std::vector<LBlock> getLblks() const & noexcept;
    void                AddToLblks(const LBlock &lblk);
    void                DelFromLblks(const size_t i);
    void                ClearLblks() noexcept;
    std::string         getID() const & noexcept;
    void                setID(const std::string &_id);
    std::string         getName() const & noexcept;
    void                setName(const std::string &_name) noexcept;
    void                fromJson(const json &j);
    json                toJson() const &;
    void                fromFile(const std::string &path);
    void                toFile(const std::string &path, const unsigned tabsize = 4);
}; // class Kit

class Map {
  private:
    std::vector<std::vector<Block>> m;

  public:
    Map() {}
    Map(const size_t w, const size_t h) {}
}; // class Map
