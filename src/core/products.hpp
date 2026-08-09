#pragma once

#include "elements.hpp"
#include <QHash>
#include <QSharedPointer>

class BasicProduct : public BasicCrtClass {
    Q_DECLARE_TR_FUNCTIONS(BasicProduct)

protected:
    QString author;
    QString des{tr("Empty")};
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
    void           toFile(const QString &path, uint8_t indent = 4);
}; // class BasicProduct

// Where stores data of blocks and large-blocks.
class Kit : public BasicProduct {
    Q_DECLARE_TR_FUNCTIONS(Kit)
    friend struct CoreStatus;

private:
    QHash<QString, QSharedPointer<Block>>  blks;
    QHash<QString, QSharedPointer<LBlock>> lblks;

public:
    Kit() = default;

    auto getBlks() const noexcept -> const QHash<QString, QSharedPointer<Block>> &;
    void clearBlks() noexcept;
    auto getLblks() const noexcept -> const QHash<QString, QSharedPointer<LBlock>> &;
    void clearLblks() noexcept;
    void operator+=(QSharedPointer<Block> blk) noexcept;
    void operator+=(QSharedPointer<LBlock> lblk) noexcept;
    void operator-=(const QString &_id);
    bool contains(const QString &_id) noexcept;
    auto operator[](const QString &_id)
        -> std::variant<QSharedPointer<Block>, QSharedPointer<LBlock>>;
    void fromJson(const json &j) override;
    json toJson() const override;
}; // class Kit

using MapDataType = QList<QList<std::variant<QSharedPointer<Block>, QSharedPointer<LBlock>>>>;

class Map : public BasicProduct {
    Q_DECLARE_TR_FUNCTIONS(Map)

private:
    MapDataType data; // nullptr: null(0) / filled by a large-block(1)

public:
    Map() = default;

    const MapDataType    &getData() const noexcept;
    void                  setData(const MapDataType &_data) noexcept;
    const Ucc             operator[](const qsizetype r, const qsizetype c);
    template <int N> auto get(const qsizetype r, const qsizetype c);
    template <class T>
    void set(const qsizetype r, const qsizetype c, const QSharedPointer<T> &element)
        requires isElem<T>
    {
        data[r][c] = element;
        // Note: (r, c) will be covered whether there's already a ucc or not
    }
    void fromJson(const json &j) override;
    json toJson() const override;
}; // class Map
