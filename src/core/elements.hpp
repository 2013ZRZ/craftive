#pragma once

#include "crtutils.hpp"
#include "i18n.hpp"
#include <QSharedPointer>
#include <variant>

struct rgb {
    Q_DECLARE_TR_FUNCTIONS(rgb)

public:
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
 *  Unicode character with background or foreground color.
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
    Q_DECLARE_TR_FUNCTIONS(Ucc)

public:
    enum class Mode : bool { b, f }; // Color mode

    char32_t c;           // Character
    rgb      b;           // Background color
    bool     hasB{false}; // Whether it has background color
    rgb      f;           // Foreground color
    bool     hasF{false}; // Whether it has foreground color

    Ucc() = default;
    Ucc(const char32_t _c) noexcept;
    Ucc(const char32_t _c, Mode cm, const rgb color) noexcept;
    Ucc(const char32_t _c, const rgb _b, const rgb _f) noexcept;
    Ucc(const isJson auto &j);

    void    fromJson(const json &j);
    json    toJson() const;
    QString operator()() const; // Get the character as QString
}; // struct Ucc

class BasicCrtClass : public QObject {
    Q_OBJECT

protected:
    QString id;
    QString name;

public:
    const QString &getID() const noexcept;
    virtual void   setID(const QString &_id);
    const QString &getName() const noexcept;
    void           setName(const QString &_name) noexcept;
    virtual void   fromJson(const json &j) = 0;
    virtual json   toJson() const          = 0;
    bool           operator==(const BasicCrtClass &other) const noexcept;
}; // class BasicCrtClass

class BasicElement : public BasicCrtClass {
    Q_OBJECT

public:
    void setID(const QString &_id) override; // An element ID should be like "kit/elem"
    // TODO Interactions
};

class Block : public BasicElement {
    Q_OBJECT
    friend class Kit;

private:
    Ucc blk;

public:
    Block() = default;
    explicit Block(const Ucc &_blk) noexcept;
    Block(const Ucc &_blk, const QString &_id, const QString &_name);
    explicit Block(const isJson auto &j);

    const Ucc &getBlk() const noexcept;
    void       setBlk(const Ucc &_blk) noexcept;
    void       fromJson(const json &j) override;
    json       toJson() const override;
}; // class Block

// Large block (a rectangular combination of blocks).
class LBlock : public BasicElement {
    Q_OBJECT
    friend class Kit;

private:
    QList<QList<Ucc>> lblk;
    QList<uint32_t>   w; // for printing for each row

public:
    LBlock() = default;
    LBlock(const qsizetype w, const qsizetype h);
    LBlock(const qsizetype w, const qsizetype h, const QString &_id, const QString &_name);
    LBlock(const QList<QList<Ucc>> &_lblk, const QString &_id, const QString &_name);
    explicit LBlock(const isJson auto &j);

    auto       getLblk() const noexcept -> const QList<QList<Ucc>> &;
    void       setLblk(const QList<QList<Ucc>> &_lblk);
    const Ucc &getPos(const qsizetype r, const qsizetype c) const;
    void       setPos(const qsizetype r, const qsizetype c, const Ucc &blk);
    uint32_t   getW(const qsizetype r) const;
    auto       getFullW() const noexcept -> const QList<uint32_t> &;
    void       setW(const qsizetype r, const uint32_t _w);
    void       fromJson(const json &j) override;
    json       toJson() const override;
}; // class LBlock

template <class T>
concept isElem = std::derived_from<std::decay_t<T>, BasicElement>;

class BasicProduct : public BasicCrtClass {
    Q_OBJECT

protected:
    QString author;
    QString des;
    Version ver;

public:
    BasicProduct() = default;
    explicit BasicProduct(const QString &path);

    const QString &getAuthor() const noexcept;
    void           setAuthor(const QString &_author);
    const QString &getDes() const noexcept;
    void           setDes(const QString &_des) noexcept;
    Version        getVer() const noexcept;
    void           setVer(Version _ver) noexcept;
    void           fromFile(const QString &path);
    void           toFile(const QString &path, const uint8_t indent = 4);
}; // class BasicProduct

// Where stores data of blocks and large-blocks.
class Kit : public BasicProduct {
    Q_OBJECT
    friend struct CoreStatus;

private:
    QHash<QStrPtr, Block>  blks;
    QHash<QStrPtr, LBlock> lblks;

public:
    Kit() = default;

    auto getBlks() const noexcept -> const QHash<QStrPtr, Block> &;
    void clearBlks() noexcept;
    auto getLblks() const noexcept -> const QHash<QStrPtr, LBlock> &;
    void clearLblks() noexcept;
    void operator+=(Block &&blk) noexcept;
    void operator+=(LBlock &&lblk) noexcept;
    void operator-=(QString &_id);
    bool contains(QString &_id) noexcept;
    auto operator[](QString &_id)
        -> const std::variant<std::reference_wrapper<Block>, std::reference_wrapper<LBlock>>;
    void fromJson(const json &j) override;
    json toJson() const override;
}; // class Kit

using MapDataType = QList<QList<std::variant<QSharedPointer<Block>, QSharedPointer<LBlock>>>>;

class Map : public BasicProduct {
    Q_OBJECT

private:
    MapDataType data; // nullptr: null(0) / filled by a large-block(1)

public:
    Map() = default;

    const MapDataType     &getData() const noexcept;
    void                   setData(const MapDataType &_data) noexcept;
    const Ucc              operator[](const qsizetype r, const qsizetype c);
    template <bool T> auto get(const qsizetype r, const qsizetype c) {}
    template <> auto       get<0>(const qsizetype r, const qsizetype c); // QSharedPointer<Block>
    template <> auto       get<1>(const qsizetype r, const qsizetype c); // QSharedPointer<LBlock>
    template <class T>
    void set(const qsizetype r, const qsizetype c, T &element)
        requires isElem<T>
    {
        data[r][c] = QSharedPointer<T>{&element};
        // Note: (r, c) will be covered whether there's already a ucc or not
    }
    void fromJson(const json &j) override;
    json toJson() const override;
}; // class Map