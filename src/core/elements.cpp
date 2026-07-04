#include "elements.hpp"
#include "crtutils.hpp"
#include "err.hpp"
#include "i18n.hpp"
#include "status.hpp"
#include <QtCore/QFile>
#include <QtCore/QStringConverter>
#include <QtCore/QTextStream>


// Definitions in rgb

rgb::rgb() noexcept : r(0), g(0), b(0) {}

rgb::rgb(uint8_t _r, uint8_t _g, uint8_t _b) noexcept : r(_r), g(_g), b(_b) {}

void rgb::fromJson(const json &j) {
    if (!j.is_array())
        throw CrtExcept(
            0x0008, tr("from rgb::fromJson(); the RGB color in the json object isn't an array"));
    if (j.size() != 3)
        throw CrtExcept(0x0008, tr("from rgb::fromJson(); incorrect number of color channels"));
    r = j[0].get<uint8_t>();
    g = j[1].get<uint8_t>();
    b = j[2].get<uint8_t>();
}

json rgb::toJson() const { return json{r, g, b}; }


// Definitions in Ucc

Ucc::Ucc(const char32_t _c) noexcept : c(_c), hasB(false), hasF(false) {}

Ucc::Ucc(const char32_t _c, const rgb _b, const rgb _f) noexcept
    : c(_c), hasB(true), b(_b), hasF(true), f(_f) {}

Ucc::Ucc(const char32_t _c, Mode cm, const rgb color) noexcept : c(_c) {
    switch (cm) {
        case Mode::b:
            hasB = true;
            b    = color;
            break;
        case Mode::f:
            hasF = true;
            f    = color;
            break;
    }
}

Ucc::Ucc(const isJson auto &j) { fromJson(j); }

void Ucc::fromJson(const json &j) {
    if (!j.is_object())
        throw CrtExcept(
            0x0006,
            tr("from Ucc::fromJson(); the unicode colored character's json isn't an object"));
    QString _c = j.at("c").get<QString>();
    if (_c.size() > sizeof(char32_t))
        throw CrtExcept(
            0x0003,
            tr("from Ucc::fromJson(); the string to parse is %1 and it has too many characters"),
            _c);
    c = _c.toStdU32String()[0];
    if (j.find("b") != j.end()) {
        hasB = true;
        b.fromJson(j["b"]);
    }
    if (j.find("f") != j.end()) {
        hasF = true;
        f.fromJson(j["f"]);
    }
}

json Ucc::toJson() const {
    json j;
    j["c"] = (*this)();
    if (hasB)
        j["b"] = b.toJson();
    if (hasF)
        j["f"] = f.toJson();
    return j;
}

QString Ucc::operator()() const { return QString::fromUcs4(&c, 1); }


// Definitions in BasicCrtClass

const QString &BasicCrtClass::getID() const noexcept { return id; }

void BasicCrtClass::setID(const QString &_id) {
    if (isInvalidID(_id))
        throw CrtExcept(0x0002,
                        tr("from BasicCrtClass::setID(); the ID includes invalid character(s)"));
    else if (_id.isEmpty())
        throw CrtExcept(0x0001, tr("from BasicCrtClass::setID()"));
    else
        id = _id;
}

const QString &BasicCrtClass::getName() const noexcept { return name; }

void BasicCrtClass::setName(const QString &_name) noexcept { name = _name; }

bool BasicCrtClass::operator==(const BasicCrtClass &other) const noexcept {
    return id == other.getID();
}


// Definitions in BasicElement

void BasicElement::setID(const QString &_id) {
    if (isInvalidElemID(_id))
        throw CrtExcept(0x0002,
                        tr("from BasicElement::setID(); the ID includes invalid character(s) or "
                           "isn't like \"kit/elem\""));
    else if (_id.isEmpty())
        throw CrtExcept(0x0001, tr("from BasicElement::setID()"));
    else
        id = _id;
}

// Definitions in Block

Block::Block(const Ucc &_blk) noexcept : blk(_blk) {}

Block::Block(const Ucc &_blk, const QString &_id, const QString &_name) {
    setBlk(_blk);
    setID(_id);
    setName(_name);
}

Block::Block(const isJson auto &j) { fromJson(j); }

const Ucc &Block::getBlk() const noexcept { return blk; }

void Block::setBlk(const Ucc &_blk) noexcept { blk = _blk; }

void Block::fromJson(const json &j) {
    if (!j.is_object())
        throw CrtExcept(0x0006, tr("from Block::fromJson(); the block's json isn't an object"));
    setBlk(Ucc{j.at("blk")});
    if (j.find("id") != j.end())
        setID(j.at("id").get<QString>());
    else
        throw CrtExcept(0x0001, tr("from Block::fromJson()"));
    setName(j.value("name", id));
}

json Block::toJson() const {
    json j{{"blk", getBlk().toJson()}, {"id", getID()}, {"name", getName()}};
    return j;
}


// Definitions in LBlock

LBlock::LBlock(const size_t _w, const size_t _h) {
    lblk.resize(_h);
    w.resize(_h);
    for (auto &i : lblk) i.resize(_w);
}

LBlock::LBlock(const size_t _w, const size_t _h, const QString &_id, const QString &_name) {
    lblk.resize(_h);
    w.resize(_h);
    for (auto &i : lblk) i.resize(_w);
    setID(_id);
    setName(_name);
}

LBlock::LBlock(const QList<QList<Ucc>> &_lblk, const QString &_id, const QString &_name) {
    setLblk(_lblk);
    setID(_id);
    setName(_name);
}

LBlock::LBlock(const isJson auto &j) { fromJson(j); }

const QList<QList<Ucc>> &LBlock::getLblk() const noexcept { return lblk; }

void LBlock::setLblk(const QList<QList<Ucc>> &_lblk) {
    lblk = _lblk;
    w.resize(lblk.size());
    for (size_t i{}; i < w.size(); i++) w[i] = lblk[i].size();
}

const Ucc &LBlock::getPos(const size_t r, const size_t c) const {
    if (r >= lblk.size() || c >= (lblk.empty() ? 0 : lblk[r].size()))
        throw CrtExcept(
            0x0005,
            tr("from LBlock::getPos(); the required position is (%1,%2), but it's out of range"),
            r,
            c);
    return lblk[r][c];
}

void LBlock::setPos(const size_t r, const size_t c, const Ucc &blk) {
    if (r >= lblk.size() || c >= (lblk.empty() ? 0 : lblk[r].size()))
        throw CrtExcept(
            0x0005,
            tr("from LBlock::setPos(); the required position is (%1,%2), but it's out of range"),
            r,
            c);
    lblk[r][c] = blk;
}

size_t LBlock::getW(const size_t r) const {
    if (r < w.size())
        return w[r];
    else if (r < lblk.size())
        return lblk[r].size();
    else
        throw CrtExcept(0x0005,
                        tr("from LBlock::getW(); the required row number is %1, but there's only "
                           "%n row(s) in this large-block",
                           nullptr,
                           lblk.size()),
                        r,
                        lblk.size());
}

auto LBlock::getFullW() const noexcept -> const QList<size_t> & { return w; }

void LBlock::setW(const size_t r, const size_t _w) {
    if (r < w.size())
        w[r] = _w;
    else
        throw CrtExcept(0x0005, tr("from LBlock::setW()"));
}

void LBlock::fromJson(const json &j) {
    if (!j.is_object())
        throw CrtExcept(0x0006,
                        tr("from LBlock::fromJson(); the large-block's json isn't an object"));
    if (!j.at("lblk").is_array())
        throw CrtExcept(0x0006, tr("from LBlock::fromJson(); the \"lblk\" isn't an array"));
    lblk.resize(j.at("lblk").size());
    w.resize(j.at("lblk").size());
    for (size_t r{}; r < j.at("lblk").size(); r++) {
        if (!j.at("lblk")[r].is_array())
            throw CrtExcept(0x0006, tr("from LBlock::fromJson(); Row %1 isn't an array"), r + 1);
        for (size_t c{}; c < j.at("lblk")[0].size(); c++) {
            lblk[r].resize(j.at("lblk")[r].size());
            setPos(r, c, Ucc{j.at("lblk")[r][c]});
        }
    }
    if (!j.at("w").is_array())
        throw CrtExcept(0x0006, tr("from LBlock::fromJson(); the \"w\" isn't an array"));
    for (size_t i{}; i < j.at("w").size(); i++) w[i] = j.at("w")[i].get<size_t>();
    if (j.find("id") != j.end())
        setID(j.at("id").get<QString>());
    else
        throw CrtExcept(0x0001, tr("from LBlock::fromJson()"));
    setName(j.value("name", id));
}

json LBlock::toJson() const {
    std::vector<std::vector<json>> lblk_json;
    lblk_json.resize(lblk.size());
    for (size_t r{}; r < lblk.size(); r++) {
        lblk_json[r].resize(lblk.empty() ? 0 : lblk[r].size());
        for (size_t c{}; c < (lblk.empty() ? 0 : lblk[r].size()); c++) {
            lblk_json[r][c] = lblk[r][c].toJson();
        }
    }
    return json{{"lblk", lblk_json}, {"w", getFullW()}, {"id", getID()}, {"name", getName()}};
}

// Definitions in BasicProduct

BasicProduct::BasicProduct(const QString &path) { fromFile(path); }

const QString &BasicProduct::getAuthor() const noexcept { return author; }

void BasicProduct::setAuthor(const QString &_author) {
    if (isInvalidEmail(_author))
        throw CrtExcept(
            0x0007,
            tr("from BasicProduct::setAuthor(); the string is \"%1\" and it isn't a valid "
               "email address"),
            _author);
    else
        author = _author;
}

const QString &BasicProduct::getDes() const noexcept { return des; }

void BasicProduct::setDes(const QString &_des) noexcept { des = _des; }

void BasicProduct::fromFile(const QString &path) {
    QFile file{path};
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        throw CrtExcept(
            0x0000,
            tr("from BasicProduct::fromFile(); failed to open the product file at %1 (%2)"),
            path,
            file.errorString());
    QTextStream in{&file};
    in.setEncoding(QStringConverter::Utf8);
    try {
        fromJson(json::parse(in.readAll()));
    } catch (const json::parse_error &e) {
        throw CrtExcept(0x0006, tr("from BasicProduct::fromFile() (%1)"), e.what());
    }
    file.close();
}

void BasicProduct::toFile(const QString &path, const uint8_t indent) {
    QFile file{path};
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        throw CrtExcept(
            0x0000,
            tr("from BasicProduct::toFile(); failed to open the product file at %1 (%2)"),
            path,
            file.errorString());
    QTextStream out{&file};
    out.setEncoding(QStringConverter::Utf8);
    out << QString::fromStdString(toJson().dump(indent));
    file.close();
}



// Definitions in Kit

auto Kit::getBlks() const noexcept -> const QHash<QStringView, Block> & { return blks; }

void Kit::clearBlks() noexcept {
    blks.clear();
    blks.squeeze();
}

auto Kit::getLblks() const noexcept -> const QHash<QStringView, LBlock> & { return lblks; }

void Kit::clearLblks() noexcept {
    lblks.clear();
    lblks.squeeze();
}

void Kit::operator+=(Block &&blk) noexcept { blks.insert(blk.id, blk); }

void Kit::operator+=(LBlock &&lblk) noexcept { lblks.insert(lblk.id, lblk); }

void Kit::operator-=(const QString &_id) {
    if (!(blks.remove(_id) || lblks.remove(_id)))
        throw CrtExcept(
            0x0004,
            tr("from Kit::operator-=; this kit doesn't contain an element who's ID is \"%1\""),
            _id);
    blks.squeeze();
    lblks.squeeze();
}

bool Kit::contains(const QString &_id) noexcept {
    return blks.contains(_id) || lblks.contains(_id);
}

auto Kit::operator[](const QString &_id) -> const std::variant<Block, LBlock> {
    if (blks.contains(_id))
        return blks[_id];
    else if (lblks.contains(_id))
        return lblks[_id];
    else
        throw CrtExcept(
            0x0004,
            tr("from Kit::operator[]; this kit doesn't contain an element who's ID is \"%1\""),
            _id);
}

void Kit::fromJson(const json &j) {
    if (!j.is_object())
        throw CrtExcept(0x0006, tr("from Kit::fromJson(); the kit's json isn't an object"));

    if (j.find("id") != j.end())
        setID(j.at("id").get<QString>());
    else
        throw CrtExcept(0x0001, tr("from Kit::fromJson(); couldn't find \"id\" in the kit's json"));

    setName(j.value("name", id));

    if (j.find("author") != j.end())
        setAuthor(j.at("author").get<QString>());
    else
        throw CrtExcept(
            0x0009,
            tr("from Kit::fromJson(); couldn't find \"author\" in the json of Kit %1 (ID: %2)"),
            name,
            id);

    setDes(j.value("des", tr("Empty")));

    if (j.find("ver") != j.end())
        setVer(Version{j.at("ver")});
    else
        throw CrtExcept(
            0x000D,
            tr("from Kit::fromJson(); couldn't find \"ver\" in the json of Kit %1 (ID: %2)"),
            name,
            id);

    if (j.find("blks") != j.end())
        for (const json &blk : j["blks"]) *this += Block{blk};
    if (j.find("lblks") != j.end())
        for (const json &lblk : j["lblks"]) *this += LBlock{lblk};
}

json Kit::toJson() const {
    json j{{"author", author}, {"id", id}, {"name", name}, {"des", des}, {"ver", ver.toJson()}};
    if (!blks.empty()) {
        std::vector<json> blks_json;
        blks_json.reserve(blks.size());
        for (const auto &i : blks) blks_json.emplace_back(i.toJson());
        j["blks"] = blks_json;
    }
    if (!lblks.empty()) {
        std::vector<json> lblks_json;
        lblks_json.reserve(lblks.size());
        for (const auto &i : lblks) lblks_json.emplace_back(i.toJson());
        j["lblks"] = lblks_json;
    }
    return j;
}


// Definitions in Map

const MapDataType &Map::getData() const noexcept { return data; }

void Map::setData(const MapDataType &_data) noexcept { data = _data; }

const Ucc Map::operator[](const size_t r, const size_t c) {
    if (r > data.size() || c > data.empty() ? 0 : data[r].size())
        throw CrtExcept(
            0x000E,
            tr("from Map::operator[]; the required position is (%1,%2), but it's out of range"),
            r,
            c);
    if (data[r][c].index() == 1) {                        // is QSharedPointer<LBlock>
        if (std::get<1>(data[r][c]) != nullptr)           // is at the upper left corner
            return std::get<1>(data[r][c])->getPos(0, 0); // getPos directly
        else {
            for (size_t _r{}; _r < r; _r++)
                for (size_t _c{}; _c < c; _c++)
                    if (data[_r][_c].index() && // (_r,_c) is QSharedPointer<LBlock>
                        std::get<1>(data[_r][_c]) !=
                            nullptr && // (_r,_c) is at the upper left corner
                        std::get<1>(data[_r][_c])->getLblk().size() + _r > r &&
                        std::get<1>(data[_r][_c])->getW(r - _r) + _c >
                            c) // (r,c) is filled by (_r,_c)
                        return std::get<1>(data[_r][_c])
                            ->getPos(r - _r, c - _c); // getPos with the relative position
            return Ucc{}; // (r,c) is null but QSharedPointer<LBlock> (UNEXCEPTED)
        }
    } else
        return std::get<0>(data[r][c]) == nullptr ? Ucc{} : std::get<0>(data[r][c])->getBlk();
}

template <> auto Map::get<0>(const size_t r, const size_t c) {
    if (r > data.size() || c > data.empty() ? 0 : data[r].size())
        throw CrtExcept(
            0x000E,
            tr("from Map::get<0>(); the required position is (%1,%2), but it's out of range"),
            r,
            c);
    return std::get<0>(data[r][c]) == nullptr ? Block{} : *std::get<0>(data[r][c]);
}

template <> auto Map::get<1>(const size_t r, const size_t c) {
    if (r > data.size() || c > data.empty() ? 0 : data[r].size())
        throw CrtExcept(
            0x000E,
            tr("from Map::get<1>(); the required position is (%1,%2), but it's out of range"),
            r,
            c);
    if (std::get<1>(data[r][c]) != nullptr) // is at the upper left corner
        return *std::get<1>(data[r][c]);
    else {
        for (size_t _r{}; _r < r; _r++)
            for (size_t _c{}; _c < c; _c++)
                if (data[_r][_c].index() &&                 // (_r,_c) is QSharedPointer<LBlock>
                    std::get<1>(data[_r][_c]) != nullptr && // (_r,_c) is at the upper left corner
                    std::get<1>(data[_r][_c])->getLblk().size() + _r > r &&
                    std::get<1>(data[_r][_c])->getW(r - _r) + _c > c) // (r,c) is filled by (_r,_c)
                    return *std::get<1>(data[_r][_c]);
        return LBlock{}; // (r,c) is null but QSharedPointer<LBlock> (UNEXCEPTED)
    }
}

void Map::fromJson(const json &j) {
    if (!j.is_object())
        throw CrtExcept(0x0006, tr("from Map::fromJson(); the map's json isn't an object"));

    if (j.find("id") != j.end())
        setID(j.at("id").get<QString>());
    else
        throw CrtExcept(0x0001, tr("from Map::fromJson(); couldn't find \"id\" in the map's json"));

    setName(j.value("name", id));

    if (j.find("author") != j.end())
        setAuthor(j.at("author").get<QString>());
    else
        throw CrtExcept(
            0x0009,
            tr("from Map::fromJson(); couldn't find \"author\" in the json of Map %1 (ID: %2)"),
            name,
            id);

    setDes(j.value("des", tr("Empty")));

    if (j.find("ver") != j.end())
        setVer(Version{j.at("ver")});
    else
        throw CrtExcept(
            0x000D,
            tr("from Map::fromJson(); couldn't find \"ver\" in the json of Map %1 (ID: %2)"),
            name,
            id);

    if (j.find("data") == j.end())
        throw CrtExcept(
            0x000A,
            tr("from Map::fromJson(); couldn't find \"data\" in the json of Map %1 (ID: %2)"),
            name,
            id);

    if (!j["data"].is_array())
        throw CrtExcept(
            0x0006,
            tr("from Map::fromJson(); \"data\" in the json of Map %1 (ID: %2) isn't an array"),
            name,
            id);

    for (size_t r{}; r < j["data"].size(); r++) {
        if (!j["data"][r].is_array())
            throw CrtExcept(0x0006,
                            tr("from Map::fromJson(); Row %1 in \"data\" in the json of Map %2 "
                               "(ID: %3) isn't an array"),
                            r,
                            name,
                            id);
        for (size_t c{}; c < j["data"][r].size(); c++) {
            if (!j["data"][r][c].is_string())
                throw CrtExcept(
                    0x0006,
                    tr("from Map::fromJson(); (%1,%2)'s ID in \"data\" in the json of Map %3 "
                       "(ID: %4) isn't a string"),
                    r,
                    c,
                    name,
                    id);
            auto elemFullID = j["data"][r][c].get<QString>();
            if (!CoreStatus::instance().containsKit(separateElemID(elemFullID).kit))
                throw CrtExcept(0x000B,
                                tr("from Map::fromJson(); couldn't find the kit (ID: %1) where "
                                   "contains the element at (%2,%3) (ID: %4) in the json of Map %5 "
                                   "(ID: %6), please load this kit and try again"),
                                separateElemID(elemFullID).kit,
                                r,
                                c,
                                elemFullID,
                                name,
                                id);
            auto &targetKit =
                CoreStatus::instance().getKit(separateElemID(j["data"][r][c].get<QString>()).kit);
            if (!targetKit.contains(elemFullID))
                throw CrtExcept(0x0004,
                                tr("from Map::fromJson(); Kit %1 (ID: %2) doesn't contain the "
                                   "element at (%3,%4) (ID: %5) in Map %6 (ID: %7)"),
                                targetKit.getName(),
                                targetKit.getID(),
                                r,
                                c,
                                elemFullID,
                                name,
                                id);
            else
                targetKit[elemFullID].visit([&](auto &&arg) { set(r, c, arg); });
        }
    }
}

json Map::toJson() const {
    json dataj;
    for (size_t r{}; r < data.size(); r++)
        for (size_t c{}; c < data[r].size(); c++)
            data[r][c].visit([&](auto &&arg) { dataj[r][c] = arg->getID(); });
    return json{{"author", author},
                {"id", id},
                {"name", name},
                {"des", des},
                {"ver", ver.toJson()},
                {"data", dataj}};
}