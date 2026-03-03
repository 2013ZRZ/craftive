#pragma once

#include "err.hpp"
#include <concepts>
#include <cstddef>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <vector>

#define BLOCKV std::vector<Block>
#define LBLOCKV std::vector<LBlock>
#define UCCV std::vector<Ucc>
#define UCCV2 std::vector<std::vector<Ucc>>

using json = nlohmann::json;

struct rgb {
    uint8_t r;
    uint8_t g;
    uint8_t b;

    rgb();
    rgb(uint8_t _r, uint8_t _g, uint8_t _b);
    std::string toB() const; // to ANSI escape code for background color
    std::string toF() const; // to ANSI escape code for foreground color
};

/** @brief Unicode character with background or foreground color.
 *
 *  Examples:
 *  Ucc(L'🤔').toStr()
 *  Ucc().cb(L'🤔', rgb(255, 0, 0).toStr()
 *  Ucc().cf(L'囧', rgb(255, 255, 0).toStr()
 *  Ucc(L'囧', rgb(255, 0, 0), rgb(255, 255, 0)).toStr()
 */
struct Ucc {
    wchar_t             c;           // Character
    bool                hasB = false; // Whether it has background color
    rgb                 b;           // Background Color
    bool                hasF = false; // Whether it has foreground color
    rgb                 f;           // Foreground Color
    mutable std::string str;

    Ucc() = default;
    Ucc(const wchar_t _c) noexcept;
    Ucc(const std::string &_str) noexcept;
    Ucc(const wchar_t _c, const rgb _b, const rgb _f) noexcept;
    Ucc               &cb(const wchar_t _c, const rgb _b) noexcept;
    Ucc               &cf(const wchar_t _c, const rgb _f) noexcept;
    const std::string &toStr() const;
};

class Element {
  protected:
    std::string id;
    std::string kit;
    std::string name;

  public:
    const std::string &getID() const noexcept;
    void               setID(const std::string &_id = randomID());
    const std::string &getKit() const noexcept;
    void               setKit(const std::string &_kit = randomID());
    const std::string &getName() const noexcept;
    void               setName(const std::string &_name) noexcept;
    virtual void       fromJson(const json &j) = 0;
    virtual json       toJson() const          = 0;
};

class Block : public Element {
  private:
    Ucc blk;

  protected:
    std::string id;
    std::string kit;
    std::string name;

  public:
    Block() = default;
    explicit Block(const Ucc &_blk) noexcept;
    Block(const Ucc         &_blk,
          const std::string &_id,
          const std::string &_kit,
          const std::string &_name);
    explicit Block(const std::same_as<json> auto &j);
    Block(const json &j, const std::string &_kit);

    const std::string &getBlk() const noexcept;
    void               setBlk(const Ucc &_blk) noexcept;
    void               fromJson(const json &j) override;
    json               toJson() const override;
}; // class Block

// Large block (a rectangular combination of blocks).
class LBlock : public Element {
  private:
    UCCV2  lblk;
    size_t w;
    size_t h; // w and h are for printing

  public:
    LBlock() = default;
    LBlock(const size_t w, const size_t h);
    LBlock(const size_t       w,
           const size_t       h,
           const std::string &_id,
           const std::string &_kit,
           const std::string &_name);
    LBlock(const UCCV2       &_lblk,
           const std::string &_id,
           const std::string &_kit,
           const std::string &_name);
    explicit LBlock(const std::same_as<json> auto &j);
    LBlock(const json &j, const std::string &_kit);

    void        setLblk(const UCCV2 &_lblk);
    std::string getLine(const size_t r) const;
    const Ucc  &getPos(const size_t r, const size_t c) const;
    void        setPos(const size_t r, const size_t c, const Ucc &blk);
    size_t      getW() const;
    void        setW(const size_t _w);
    size_t      getH() const;
    void        setH(const size_t _h);
    void        fromJson(const json &j) override;
    json        toJson() const override;
}; // class LBlock

// Where stores data of blocks and large-blocks.
class Kit final {
  private:
    std::string author;
    BLOCKV      blks;
    LBLOCKV     lblks;
    std::string id;
    std::string name;

  public:
    Kit() = default;
    explicit Kit(const std::filesystem::path &path);

    const std::string &getAuthor() const noexcept;
    void               setAuthor(const std::string &_author) noexcept;
    const BLOCKV      &getBlks() const noexcept;
    void               clearBlks() noexcept;
    const LBLOCKV     &getLblks() const noexcept;
    void               clearLblks() noexcept;
    void               operator+=(const Block &blk);
    void               operator+=(const LBlock &lblk);
    void               operator-=(const std::string &_id);
    const std::string &getID() const noexcept;
    void               setID(const std::string &_id);
    const std::string &getName() const noexcept;
    void               setName(const std::string &_name) noexcept;
    void               fromJson(const json &j);
    json               toJson() const;
    void               fromFile(const std::filesystem::path &path);
    void               toFile(const std::filesystem::path &path, const unsigned tabsize = 4);
}; // class Kit
