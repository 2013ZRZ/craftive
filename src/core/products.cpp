#include "products.hpp"
#include "err.hpp"
#include "status.hpp"
#include <QFile>
#include <QTextStream>


// Definitions in BasicProduct

BasicProduct::BasicProduct(const QString &path) { fromFile(path); }

const QString &BasicProduct::getAuthor() const noexcept { return author; }

void BasicProduct::setAuthor(const QString &_author) {
    try {
        checkEmail(_author);
        author = _author;
    }
    CATCH_THROW(context)
}

const QString &BasicProduct::getDes() const noexcept { return des; }

void BasicProduct::setDes(const QString &_des) noexcept { des = _des; }

Version BasicProduct::getVer() const noexcept { return ver; }

void BasicProduct::setVer(Version _ver) noexcept { ver = _ver; }

void BasicProduct::fromFile(const QString &path) {
    try {
        QFile file{path};
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            throw CrtExcept(
                0x0000,
                tr("From BasicProduct::fromFile(); failed to open the product file at %1 (%2)"),
                path,
                file.errorString());
        QTextStream in{&file};
        in.setEncoding(QStringConverter::Utf8);
        fromJson(json::parse(in.readAll().toStdString()));
        file.close();
    }
    CATCH_THROW(BasicProduct::fromFile())
}

void BasicProduct::toFile(const QString &path, uint8_t indent) {
    try {
        QFile file{path};
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            throw CrtExcept(
                0x0000,
                tr("From BasicProduct::toFile(); failed to open the product file at %1 (%2)"),
                path,
                file.errorString());
        QTextStream out{&file};
        out.setEncoding(QStringConverter::Utf8);
        out << QString::fromStdString(toJson().dump(indent));
        file.close();
    }
    CATCH_THROW(BasicProduct::toFile())
}



// Definitions in Kit

auto Kit::getBlks() const noexcept -> const QHash<QString, QSharedPointer<Block>> & { return blks; }

void Kit::clearBlks() noexcept { blks.clear(); }

auto Kit::getLblks() const noexcept -> const QHash<QString, QSharedPointer<LBlock>> & {
    return lblks;
}

void Kit::clearLblks() noexcept { lblks.clear(); }

void Kit::operator+=(QSharedPointer<Block> blk) noexcept { blks.insert(blk->getID(), blk); }

void Kit::operator+=(QSharedPointer<LBlock> lblk) noexcept { lblks.insert(lblk->getID(), lblk); }

void Kit::operator-=(const QString &_id) {
    if (!(blks.remove(_id) || lblks.remove(_id)))
        throw CrtExcept(
            0x0004,
            tr("From Kit::operator-=; this kit doesn't contain an element who's ID is \"%1\""),
            _id);
}

bool Kit::contains(const QString &_id) noexcept {
    return blks.contains(_id) || lblks.contains(_id);
}

auto Kit::operator[](const QString &_id)
    -> std::variant<QSharedPointer<Block>, QSharedPointer<LBlock>> {
    if (blks.contains(_id))
        return blks[_id];
    else if (lblks.contains(_id))
        return lblks[_id];
    else
        throw CrtExcept(
            0x0004,
            tr("From Kit::operator[]; this kit doesn't contain an element who's ID is \"%1\""),
            _id);
}

void Kit::fromJson(const json &j) {
    try {
        if (!j.is_object())
            throw CrtExcept(0x0006, tr("From Kit::fromJson(); the kit's JSON isn't an object"));

        // id
        if (j.find("id") != j.end())
            setID(j.at("id").get<QString>());
        else
            throw CrtExcept(0x0001,
                            tr("From Kit::fromJson(); couldn't find \"id\" in the kit's JSON"));

        // name
        setName(j.value("name", id));

        // author
        if (j.find("author") != j.end())
            setAuthor(j.at("author").get<QString>());
        else
            throw CrtExcept(
                0x0009,
                tr("From Kit::fromJson(); couldn't find \"author\" in the JSON of Kit %1 (ID: %2)"),
                name,
                id);

        // des
        setDes(j.value("des", QCoreApplication::translate("BasicProduct", "Empty")));

        // ver
        if (j.find("ver") != j.end())
            setVer(Version{j.at("ver")});
        else
            throw CrtExcept(
                0x000D,
                tr("From Kit::fromJson(); couldn't find \"ver\" in the JSON of Kit %1 (ID: %2)"),
                name,
                id);

        // blks
        if (j.find("blks") != j.end())
            for (const json &blk : j["blks"]) *this += QSharedPointer<Block>{new Block{blk}};

        // lblks
        if (j.find("lblks") != j.end())
            for (const json &lblk : j["lblks"]) *this += QSharedPointer<LBlock>{new LBlock{lblk}};
    }
    CATCH_THROW(Kit::fromJson())
}

json Kit::toJson() const {
    try {
        json j{{"author", author}, {"id", id}, {"name", name}, {"des", des}, {"ver", ver.toJson()}};
        if (!blks.empty()) {
            std::vector<json> blks_json;
            blks_json.reserve(blks.size());
            for (const auto &i : blks) blks_json.emplace_back(i->toJson());
            j["blks"] = blks_json;
        }
        if (!lblks.empty()) {
            std::vector<json> lblks_json;
            lblks_json.reserve(lblks.size());
            for (const auto &i : lblks) lblks_json.emplace_back(i->toJson());
            j["lblks"] = lblks_json;
        }
        return j;
    }
    CATCH_THROW(Kit::toJson())
}


// Definitions in Map

const MapDataType &Map::getData() const noexcept { return data; }

void Map::setData(const MapDataType &_data) noexcept { data = _data; }

const Ucc Map::operator[](const qsizetype r, const qsizetype c) {
    try {
        if (r >= data.size() || c >= (data.empty() ? 0 : data[r].size()))
            throw CrtExcept(
                0x000E,
                tr("From Map::operator[]; the required position is (%1,%2), but it's out of range"),
                r,
                c);
        if (data[r][c].index() == 1) {                        // is QSharedPointer<LBlock>
            if (std::get<1>(data[r][c]) != nullptr)           // is at the upper left corner
                return std::get<1>(data[r][c])->getPos(0, 0); // getPos directly
            else {
                for (qsizetype _r{0}; _r < r; _r++)
                    for (qsizetype _c{0}; _c < c; _c++)
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
    CATCH_THROW(Map::operator[])
}

template <> auto Map::get<0>(const qsizetype r, const qsizetype c) {
    try {
        if (r >= data.size() || c >= (data.empty() ? 0 : data[r].size()))
            throw CrtExcept(
                0x000E,
                Map::tr(
                    "From Map::get<0>(); the required position is (%1,%2), but it's out of range"),
                r,
                c);
        return std::get<0>(data[r][c]) == QSharedPointer<Block>{nullptr}
                   ? QSharedPointer<Block>{nullptr}
                   : std::get<0>(data[r][c]);
    }
    CATCH_THROW(Map::get<0>())
}

template <> auto Map::get<1>(const qsizetype r, const qsizetype c) {
    try {
        if (r >= data.size() || c >= (data.empty() ? 0 : data[r].size()))
            throw CrtExcept(
                0x000E,
                Map::tr(
                    "From Map::get<1>(); the required position is (%1,%2), but it's out of range"),
                r,
                c);
        if (std::get<1>(data[r][c]) != nullptr) // is at the upper left corner
            return std::get<1>(data[r][c]);
        else {
            for (qsizetype _r{0}; _r < r; _r++)
                for (qsizetype _c{0}; _c < c; _c++)
                    if (data[_r][_c].index() == 1) { // (_r,_c) is QSharedPointer<LBlock>
                        auto &target = std::get<1>(data[_r][_c]);
                        if (target != nullptr && // (_r,_c) is at the upper left corner
                            target->getLblk().size() + _r > r &&
                            target->getW(r - _r) + _c > c) // (r,c) is filled by (_r,_c)
                            return std::get<1>(data[_r][_c]);
                    }
            // (r,c) is null but QSharedPointer<LBlock> (UNEXCEPTED)
            return QSharedPointer<LBlock>{nullptr};
        }
    }
    CATCH_THROW(Map::get<1>())
}

void Map::fromJson(const json &j) {
    try {
        if (!j.is_object())
            throw CrtExcept(0x0006, tr("From Map::fromJson(); the map's JSON isn't an object"));

        // id
        if (j.find("id") != j.end())
            setID(j.at("id").get<QString>());
        else
            throw CrtExcept(0x0001,
                            tr("From Map::fromJson(); couldn't find \"id\" in the map's JSON"));

        // name
        setName(j.value("name", id));

        // author
        if (j.find("author") != j.end())
            setAuthor(j.at("author").get<QString>());
        else
            throw CrtExcept(
                0x0009,
                tr("From Map::fromJson(); couldn't find \"author\" in the JSON of Map %1 (ID: %2)"),
                name,
                id);

        // des
        setDes(j.value("des", QCoreApplication::translate("BasicProduct", "Empty")));

        // ver
        if (j.find("ver") != j.end())
            setVer(Version{j.at("ver")});
        else
            throw CrtExcept(
                0x000D,
                tr("From Map::fromJson(); couldn't find \"ver\" in the JSON of Map %1 (ID: %2)"),
                name,
                id);

        // data
        if (j.find("data") == j.end())
            throw CrtExcept(
                0x000A,
                tr("From Map::fromJson(); couldn't find \"data\" in the JSON of Map %1 (ID: %2)"),
                name,
                id);
        if (!j["data"].is_array())
            throw CrtExcept(
                0x0006,
                tr("From Map::fromJson(); \"data\" in the JSON of Map %1 (ID: %2) isn't an array"),
                name,
                id);
        for (size_t r{0}; r < j["data"].size(); r++) {
            if (!j["data"][r].is_array())
                throw CrtExcept(0x0006,
                                tr("From Map::fromJson(); Row %1 in \"data\" in the JSON of Map %2 "
                                   "(ID: %3) isn't an array"),
                                r,
                                name,
                                id);
            for (size_t c{0}; c < j["data"][r].size(); c++) {
                if (!j["data"][r][c].is_string())
                    throw CrtExcept(
                        0x0006,
                        tr("From Map::fromJson(); (%1,%2)'s ID in \"data\" in the JSON of Map %3 "
                           "(ID: %4) isn't a string"),
                        r,
                        c,
                        name,
                        id);
                auto elemFullID  = j["data"][r][c].get<QString>();
                auto targetKitID = separateElemID(elemFullID).kit;
                if (!CoreStatus::instance().containsKit(targetKitID))
                    throw CrtExcept(
                        0x000B,
                        tr("From Map::fromJson(); couldn't find the kit (ID: %1) where "
                           "contains the element at (%2,%3) (ID: %4) in the JSON of Map %5 "
                           "(ID: %6), please load this kit and try again"),
                        targetKitID,
                        r,
                        c,
                        elemFullID,
                        name,
                        id);
                auto &targetKit = CoreStatus::instance().getKit(targetKitID);
                if (!targetKit.contains(elemFullID))
                    throw CrtExcept(0x0004,
                                    tr("From Map::fromJson(); Kit %1 (ID: %2) doesn't contain the "
                                       "element at (%3,%4) (ID: %5) in Map %6 (ID: %7)"),
                                    targetKit.getName(),
                                    targetKitID,
                                    r,
                                    c,
                                    elemFullID,
                                    name,
                                    id);
                else
                    std::visit([&](auto &&arg) { set(r, c, arg); }, targetKit[elemFullID]);
            }
        }
    }
    CATCH_THROW(Map::fromJson())
}

json Map::toJson() const {
    try {
        json dataj;
        for (qsizetype r{0}; r < data.size(); r++)
            for (qsizetype c{0}; c < data[r].size(); c++)
                std::visit([&](auto &&arg) { dataj[r][c] = arg->getID(); }, data[r][c]);
        return json{{"author", author},
                    {"id", id},
                    {"name", name},
                    {"des", des},
                    {"ver", ver.toJson()},
                    {"data", dataj}};
    }
    CATCH_THROW(Map::toJson())
}
