#pragma once

#include "crtutils.hpp"
#include <QtCore/QSharedPointer>
#include <variant>

struct rgb {
    uint8_t r;
    uint8_t g;
    uint8_t b;

    rgb() noexcept;
    rgb(uint8_t _r, uint8_t _g, uint8_t _b) noexcept;
    void fromJson(const json &j);
    json toJson() const;
}; // struct rgb

/** Unicode Colored Character
 *
 *  @brief Unicode character with background or foreground color.
 *
 *  U'\t' means a null character.
 *
 *  Examples:
 *  Ucc{U'🤔'}
 *  Ucc{U'🤔', Ucc::Mode::b, rgb{255, 0, 0}}
 *  Ucc{U'囧', Ucc::Mode::f, rgb{255, 255, 0}}
 *  Ucc{U'囧', rgb{255, 0, 0}, rgb{255, 255, 0}}
 */
struct Ucc {
    enum class Mode : bool { b, f }; // Color mode

    char32_t c;                // Character
    rgb      b;                // Background color
    rgb      f;                // Foreground color
    bool     hasB : 1 {false}; // Whether it has background color
    bool     hasF : 1 {false}; // Whether it has foreground color

    Ucc() = default;
    Ucc(const char32_t _c) noexcept;
    Ucc(const char32_t _c, Mode cm, const rgb color) noexcept;
    Ucc(const char32_t _c, const rgb _b, const rgb _f) noexcept;
    Ucc(const is_json auto &j);
    void    fromJson(const json &j);
    json    toJson() const;
    QString operator()() const; // Get the character as QString
} __attribute__((__packed__));  // struct Ucc

using UccL  = QList<Ucc>;
using UccL2 = QList<QList<Ucc>>;

class BasicCrtClass {
  protected:
    QString id;
    QString name;

  public:
    const QString &getID() const noexcept;
    void           setID(const QString &_id);
    const QString &getName() const noexcept;
    void           setName(const QString &_name) noexcept;
    virtual void   fromJson(const json &j) = 0;
    virtual json   toJson() const          = 0;
    bool           operator==(const BasicCrtClass &other) const noexcept;
}; // class BasicCrtClass

class BasicElement : public BasicCrtClass {}; // TODO Interactions

class Block : public BasicElement {
  private:
    Ucc blk;

  public:
    Block() = default;
    explicit Block(const Ucc &_blk) noexcept;
    Block(const Ucc &_blk, const QString &_id, const QString &_name);
    explicit Block(const is_json auto &j);

    const Ucc &getBlk() const noexcept;
    void       setBlk(const Ucc &_blk) noexcept;
    void       fromJson(const json &j) override;
    json       toJson() const override;
}; // class Block

using BlockL = QList<Block>;

// Large block (a rectangular combination of blocks).
class LBlock : public BasicElement {
  private:
    UccL2         lblk;
    QList<size_t> w; // for printing for each row

  public:
    LBlock() = default;
    LBlock(const size_t w, const size_t h);
    LBlock(const size_t w, const size_t h, const QString &_id, const QString &_name);
    LBlock(const UccL2 &_lblk, const QString &_id, const QString &_name);
    explicit LBlock(const is_json auto &j);

    const UccL2 &getLblk() const noexcept;
    void         setLblk(const UccL2 &_lblk);
    const Ucc   &getPos(const size_t r, const size_t c) const;
    void         setPos(const size_t r, const size_t c, const Ucc &blk);
    size_t       getW(const size_t r) const;
    auto         getFullW() const noexcept -> const QList<size_t> &;
    void         setW(const size_t r, const size_t _w);
    void         fromJson(const json &j) override;
    json         toJson() const override;
}; // class LBlock

using LBlockL = QList<LBlock>;

class BasicProduct : public BasicCrtClass {
  protected:
    QString  author;
    QString  des;
    uint32_t price;

  public:
    BasicProduct() = default;
    const QString &getAuthor() const noexcept;
    void           setAuthor(const QString &_author);
    const QString &getDes() const noexcept;
    void           setDes(const QString &_des) noexcept;
    uint32_t       getPrice() const noexcept;
    void           setPrice(uint32_t _price) noexcept;
    virtual void   fromFile(const QString &path)                          = 0;
    virtual void   toFile(const QString &path, const uint8_t tabsize = 4) = 0;
}; // class BasicProduct

// Where stores data of blocks and large-blocks.
class Kit : public BasicProduct {
  private:
    BlockL  blks;
    LBlockL lblks;

  public:
    Kit() = default;
    explicit Kit(const QString &path);

    const BlockL  &getBlks() const noexcept;
    void           clearBlks() noexcept;
    const LBlockL &getLblks() const noexcept;
    void           clearLblks() noexcept;
    void           operator+=(const Block &blk);
    void           operator+=(const LBlock &lblk);
    void           operator-=(const QString &_id);
    auto           operator[](const QString &_id) -> const std::variant<Block, LBlock>;
    void           fromJson(const json &j) override;
    json           toJson() const override;
    void           fromFile(const QString &path) override;
    void           toFile(const QString &path, const uint8_t tabsize = 4) override;
}; // class Kit

using MapDataType = QList<QList<std::variant<QSharedPointer<Block>, QSharedPointer<LBlock>>>>;

class Map : public BasicProduct {
  private:
    MapDataType    data; // nullptr: null(0) / filled by a large-block(1)
    QList<QString> deps; // Kit dependences

  public:
    Map() = default;
    explicit Map(const QString &path);

    const MapDataType     &getData() const noexcept;
    void                   setData(const MapDataType &_data) noexcept;
    const Ucc              operator[](const size_t r, const size_t c);
    template <bool T> auto get(const size_t r, const size_t c) {}
    template <> auto       get<0>(const size_t r, const size_t c); // Block
    template <> auto       get<1>(const size_t r, const size_t c); // LBlock
    void                   fromJson(const json &j) override;
    json                   toJson() const override;
    void                   fromFile(const QString &path) override;
    void                   toFile(const QString &path, const uint8_t tabsize = 4) override;
}; // class Map