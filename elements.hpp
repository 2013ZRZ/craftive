#pragma once

#include "crtutils.hpp"
#include <filesystem>
#include <vector>

struct rgb {
    uint8_t r;
    uint8_t g;
    uint8_t b;

    rgb() noexcept;
    rgb(uint8_t _r, uint8_t _g, uint8_t _b) noexcept;
    void fromJson(const json &j);
    json toJson() const;
};

/** Unicode Colored Character
 *
 *  @brief Unicode character with background or foreground color.
 *
 *  Examples:
 *  Ucc(U'🤔')
 *  Ucc().cb(U'🤔', rgb(255, 0, 0)
 *  Ucc().cf(U'囧', rgb(255, 255, 0)
 *  Ucc(U'囧', rgb(255, 0, 0), rgb(255, 255, 0))
 */
struct Ucc {
    char32_t c;            // Character
    bool     hasB = false; // Whether it has background color
    rgb      b;            // Background Color
    bool     hasF = false; // Whether it has foreground color
    rgb      f;            // Foreground Color

    Ucc() = default;
    Ucc(const char32_t _c) noexcept;
    Ucc(const is_json auto &j);
    Ucc(const char32_t _c, const rgb _b, const rgb _f) noexcept;
    Ucc        &cb(const char32_t _c, const rgb _b) noexcept;
    Ucc        &cf(const char32_t _c, const rgb _f) noexcept;
    void        fromJson(const json &j);
    json        toJson() const;
    std::string operator()() const; // Get the character as std::string
};

using UccV  = std::vector<Ucc>;
using UccV2 = std::vector<std::vector<Ucc>>;

class Element {
  protected:
    std::string id;
    std::string name;

  public:
    const std::string &getID() const noexcept;
    void               setID(const std::string &_id = randomID());
    const std::string &getName() const noexcept;
    void               setName(const std::string &_name) noexcept;
    virtual void       fromJson(const json &j) = 0;
    virtual json       toJson() const          = 0;
    bool               operator==(const Element &other) const noexcept;
};

class Block : public Element {
  private:
    Ucc blk;

  public:
    Block() = default;
    explicit Block(const Ucc &_blk) noexcept;
    Block(const Ucc &_blk, const std::string &_id, const std::string &_name);
    explicit Block(const is_json auto &j);

    const Ucc &getBlk() const noexcept;
    void       setBlk(const Ucc &_blk) noexcept;
    void       fromJson(const json &j) override;
    json       toJson() const override;
}; // class Block

using BlockV = std::vector<Block>;

// Large block (a rectangular combination of blocks).
class LBlock : public Element {
  private:
    UccV2  lblk;
    size_t w;
    size_t h; // w and h are for printing

  public:
    LBlock() = default;
    LBlock(const size_t w, const size_t h);
    LBlock(const size_t w, const size_t h, const std::string &_id, const std::string &_name);
    LBlock(const UccV2 &_lblk, const std::string &_id, const std::string &_name);
    explicit LBlock(const is_json auto &j);

    const UccV2 &getLblk() const noexcept;
    void         setLblk(const UccV2 &_lblk);
    const Ucc   &getPos(const size_t r, const size_t c) const;
    void         setPos(const size_t r, const size_t c, const Ucc &blk);
    size_t       getW() const;
    void         setW(const size_t _w);
    size_t       getH() const;
    void         setH(const size_t _h);
    void         fromJson(const json &j) override;
    json         toJson() const override;
}; // class LBlock

using LBlockV = std::vector<LBlock>;

// Where stores data of blocks and large-blocks.
class Kit final {
  private:
    std::string author;
    BlockV      blks;
    LBlockV     lblks;
    std::string id;
    std::string name;
    std::string des;

  public:
    Kit() = default;
    explicit Kit(const std::filesystem::path &path);

    const std::string &getAuthor() const noexcept;
    void               setAuthor(const std::string &_author) noexcept;
    const BlockV      &getBlks() const noexcept;
    void               clearBlks() noexcept;
    const LBlockV     &getLblks() const noexcept;
    void               clearLblks() noexcept;
    void               operator+=(const Block &blk);
    void               operator+=(const LBlock &lblk);
    void               operator-=(const std::string &_id);
    const std::string &getID() const noexcept;
    void               setID(const std::string &_id);
    const std::string &getName() const noexcept;
    void               setName(const std::string &_name) noexcept;
    const std::string &getDes() const noexcept;
    void               setDes(const std::string &_des) noexcept;
    void               fromJson(const json &j);
    json               toJson() const;
    void               fromFile(const std::filesystem::path &path);
    void               toFile(const std::filesystem::path &path, const unsigned tabsize = 4);
}; // class Kit
