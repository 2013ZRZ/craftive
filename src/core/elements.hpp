#pragma once

#include "crtutils.hpp"

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
 *  Unicode character with background and / or foreground color.
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

class BasicCrtClass {
    Q_DECLARE_TR_FUNCTIONS(BasicCrtClass)

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
    Q_DECLARE_TR_FUNCTIONS(BasicElement)

public:
    void setID(const QString &_id) override; // An element ID should be like "kit/elem"
    // TODO Interactions
};

class Block : public BasicElement {
    Q_DECLARE_TR_FUNCTIONS(Block)
    friend class Kit;

private:
    Ucc blk;

public:
    Block() = default;
    explicit Block(const Ucc &_blk) noexcept;
    Block(const Ucc &_blk, const QString &_id, const QString &_name);
    explicit Block(const isJson auto &j) { fromJson(j); }

    const Ucc &getBlk() const noexcept;
    void       setBlk(const Ucc &_blk) noexcept;
    void       fromJson(const json &j) override;
    json       toJson() const override;
}; // class Block

// Large block (a rectangular combination of blocks).
class LBlock : public BasicElement {
    Q_DECLARE_TR_FUNCTIONS(LBlock)
    friend class Kit;

private:
    QList<QList<Ucc>> lblk;
    QList<uint32_t>   w; // for printing for each row

public:
    LBlock() = default;
    LBlock(const qsizetype _w, const qsizetype _h);
    LBlock(const qsizetype _w, const qsizetype _h, const QString &_id, const QString &_name);
    LBlock(const QList<QList<Ucc>> &_lblk, const QString &_id, const QString &_name);
    explicit LBlock(const isJson auto &j) { fromJson(j); }

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