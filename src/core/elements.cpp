#include "elements.hpp"
#include "err.hpp"


// Definitions in rgb

rgb::rgb() noexcept : r(0), g(0), b(0) {}

rgb::rgb(uint8_t _r, uint8_t _g, uint8_t _b) noexcept : r(_r), g(_g), b(_b) {}

void rgb::fromJson(const json &j) {
    try {
        if (!j.is_array())
            throw CrtExcept(
                0x0008,
                tr("From rgb::fromJson(); the RGB color in the JSON object isn't an array"));
        if (j.size() != 3)
            throw CrtExcept(0x0008, tr("From rgb::fromJson(); incorrect number of color channels"));
        r = j[0].get<uint8_t>();
        g = j[1].get<uint8_t>();
        b = j[2].get<uint8_t>();
    }
    CATCH_THROW(rgb::fromJson())
}

json rgb::toJson() const { return json{r, g, b}; }

// Definitions in Ucc

Ucc::Ucc(const char32_t _c) noexcept : c(_c), hasB(false), hasF(false) {}

Ucc::Ucc(const char32_t _c, const rgb _b, const rgb _f) noexcept
    : c(_c), b(_b), hasB(true), f(_f), hasF(true) {}

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
    try {
        if (!j.is_object())
            throw CrtExcept(
                0x0006,
                tr("From Ucc::fromJson(); the unicode colored character's JSON isn't an object"));
        QString _c = j.at("c").get<QString>();
        if (static_cast<size_t>(_c.size()) > sizeof(char32_t))
            throw CrtExcept(0x0003,
                            tr("From Ucc::fromJson(); the string to parse is %1 and it has too "
                               "many characters"),
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
    CATCH_THROW(Ucc::fromJson())
}

json Ucc::toJson() const {
    try {
        json j;
        j["c"] = (*this)();
        if (hasB)
            j["b"] = b.toJson();
        if (hasF)
            j["f"] = f.toJson();
        return j;
    }
    CATCH_THROW(Ucc::toJson())
}

QString Ucc::operator()() const {
    try {
        return QString::fromUcs4(&c, 1);
    }
    CATCH_THROW(Ucc::operator())
}


// Definitions in BasicCrtClass

const QString &BasicCrtClass::getID() const noexcept { return id; }

void BasicCrtClass::setID(const QString &_id) {
    try {
        checkID(_id);
        id = _id;
    }
    CATCH_THROW(BasicCrtClass::setID())
}

const QString &BasicCrtClass::getName() const noexcept { return name; }

void BasicCrtClass::setName(const QString &_name) noexcept { name = _name; }

bool BasicCrtClass::operator==(const BasicCrtClass &other) const noexcept {
    return id == other.getID();
}


// Definitions in BasicElement

void BasicElement::setID(const QString &_id) {
    try {
        checkElemID(_id);
        id = _id;
    }
    CATCH_THROW(BasicElement::setID())
}

// Definitions in Block

Block::Block(const Ucc &_blk) noexcept : blk(_blk) {}

Block::Block(const Ucc &_blk, const QString &_id, const QString &_name) {
    setBlk(_blk);
    setID(_id);
    setName(_name);
}

const Ucc &Block::getBlk() const noexcept { return blk; }

void Block::setBlk(const Ucc &_blk) noexcept { blk = _blk; }

void Block::fromJson(const json &j) {
    try {
        if (!j.is_object())
            throw CrtExcept(0x0006, tr("From Block::fromJson(); the block's JSON isn't an object"));

        // id
        if (j.find("id") != j.end())
            setID(j.at("id").get<QString>());
        else
            throw CrtExcept(0x0001,
                            tr("From Block::fromJson(); couldn't find \"id\" in the block's JSON"));

        // name
        setName(j.value("name", id));

        // blk
        if (j.find("blk") != j.end())
            setBlk(Ucc{j.at("blk")});
        else
            throw CrtExcept(0x000A,
                            tr("From Block::fromJson(); couldn't find \"blk\" in the JSON of the "
                               "block %1 (ID: %2)"),
                            name,
                            id);
    }
    CATCH_THROW(Block::fromJson())
}

json Block::toJson() const {
    try {
        json j{{"blk", getBlk().toJson()}, {"id", getID()}, {"name", getName()}};
        return j;
    }
    CATCH_THROW(Block::toJson())
}


// Definitions in LBlock

LBlock::LBlock(const qsizetype _w, const qsizetype _h) {
    lblk.resize(_h);
    w.resize(_h);
    for (auto &i : lblk) i.resize(_w);
}

LBlock::LBlock(const qsizetype _w, const qsizetype _h, const QString &_id, const QString &_name) {
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

const QList<QList<Ucc>> &LBlock::getLblk() const noexcept { return lblk; }

void LBlock::setLblk(const QList<QList<Ucc>> &_lblk) {
    try {
        lblk = _lblk;
        w.resize(lblk.size());
        for (qsizetype i{0}; i < w.size(); i++) w[i] = lblk[i].size();
    }
    CATCH_THROW(LBlock::setLblk())
}

const Ucc &LBlock::getPos(const qsizetype r, const qsizetype c) const {
    try {
        if (r >= lblk.size() || c >= (lblk.empty() ? 0 : lblk[r].size()))
            throw CrtExcept(0x0005,
                            tr("From LBlock::getPos(); the required position is (%1,%2), but it's "
                               "out of range"),
                            r,
                            c);
        return lblk[r][c];
    }
    CATCH_THROW(LBlock::getPos())
}

void LBlock::setPos(const qsizetype r, const qsizetype c, const Ucc &blk) {
    try {
        if (r >= lblk.size() || c >= (lblk.empty() ? 0 : lblk[r].size()))
            throw CrtExcept(0x0005,
                            tr("From LBlock::setPos(); the required position is (%1,%2), but it's "
                               "out of range"),
                            r,
                            c);
        lblk[r][c] = blk;
    }
    CATCH_THROW(LBlock::setPos())
}

uint32_t LBlock::getW(const qsizetype r) const {
    try {
        if (r < w.size())
            return w[r];
        else if (r < lblk.size())
            return lblk[r].size();
        else
            throw CrtExcept(
                0x0005,
                tr("From LBlock::getW(); the required row number is %1, but there's only "
                   "%n row(s) in this large-block",
                   nullptr,
                   lblk.size()),
                r,
                lblk.size());
    }
    CATCH_THROW(Lblock::getW())
}

auto LBlock::getFullW() const noexcept -> const QList<uint32_t> & { return w; }

void LBlock::setW(const qsizetype r, const uint32_t _w) {
    try {
        if (r < w.size())
            w[r] = _w;
        else
            throw CrtExcept(0x0005, tr("From LBlock::setW()"));
    }
    CATCH_THROW(LBlock::setW())
}

void LBlock::fromJson(const json &j) {
    try {
        if (!j.is_object())
            throw CrtExcept(0x0006,
                            tr("From LBlock::fromJson(); the large-block's JSON isn't an object"));

        // id
        if (j.find("id") != j.end())
            setID(j.at("id").get<QString>());
        else
            throw CrtExcept(
                0x0001,
                tr("From LBlock::fromJson(); couldn't find \"id\" in the large-block's JSON"));

        // name
        setName(j.value("name", id));

        // lblk
        if (j.find("lblk") != j.end()) {
            if (!j.at("lblk").is_array())
                throw CrtExcept(
                    0x0006,
                    tr("From LBlock::fromJson(); the \"lblk\" isn't an array in the JSON "
                       "of the large-block %1 (ID: %2)"),
                    name,
                    id);
            lblk.resize(j.at("lblk").size());
            for (size_t r{0}; r < j.at("lblk").size(); r++) {
                if (!j.at("lblk")[r].is_array())
                    throw CrtExcept(
                        0x0006,
                        tr("From LBlock::fromJson(); Row %1 isn't an array in the JSON of "
                           "the large-block %2 (ID: %3)"),
                        r + 1,
                        name,
                        id);
                for (size_t c{0}; c < j.at("lblk")[0].size(); c++) {
                    lblk[r].resize(j.at("lblk")[r].size());
                    setPos(r, c, Ucc{j.at("lblk")[r][c]});
                }
            }
        } else
            throw CrtExcept(0x000A,
                            tr("From LBlock::fromJson(); couldn't find \"lblk\" in the JSON of the "
                               "large-block %1 (ID: %2)"),
                            name,
                            id);

        // w
        if (!j.at("w").is_array())
            throw CrtExcept(
                0x0006,
                tr("From LBlock::fromJson(); the \"w\" isn't an array in the JSON of the "
                   "large-block %1 (ID: %2)"),
                name,
                id);
        w.resize(lblk.size());
        for (size_t i{0}; i < j.at("w").size(); i++) w[i] = j.at("w")[i].get<size_t>();
    }
    CATCH_THROW(LBlock::fromJson())
}

json LBlock::toJson() const {
    try {
        std::vector<std::vector<json>> lblk_json;
        lblk_json.resize(lblk.size());
        for (qsizetype r{0}; r < lblk.size(); r++) {
            lblk_json[r].resize(lblk.empty() ? 0 : lblk[r].size());
            for (qsizetype c{0}; c < (lblk.empty() ? 0 : lblk[r].size()); c++) {
                lblk_json[r][c] = lblk[r][c].toJson();
            }
        }
        return json{{"lblk", lblk_json}, {"w", getFullW()}, {"id", getID()}, {"name", getName()}};
    }
    CATCH_THROW(LBlock::toJson())
}