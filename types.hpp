#pragma once

#include <cstddef>
#include "err.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>
#include <vector>

using json = nlohmann::json;

class block {
private:
  std::string blk;
  std::string id;
  std::string kit;
  std::string name;

public:
  block() {
    setBlk("null");
  }
  block(const unsigned short back) {
    setBlk("null", back);
  }
  block(const std::string& blk_) {
    setBlk(blk_);
  }
  block(
    const std::string& blk_,
    const std::string& id_,
    const std::string& kit_,
    const std::string& name_
  ) {
    setBlk(blk_);
    setID(id_);
    setKit(kit_);
    setName(name_);
  }
  block(
    const std::string& blk_,
    const unsigned short back,
    const std::string& id_,
    const std::string& kit_,
    const std::string& name_
  ) {
    setBlk(blk_, back);
    setID(id_);
    setKit(kit_);
    setName(name_);
  }
  block(const json& j) {
    fromJson(j);
  }
  block(const json& j, const std::string& kit_) {
    fromJson(j);
    setKit(kit_);
  }
  
  std::string getBlk() const& {
    return blk;
  }
  void setBlk(const std::string& blk_) {
    blk = (blk_ == "null" ? "  " : blk_);
  }
  void setBlk(const std::string& blk_, const unsigned short back) {
    if (back > 255) {
      throw errcode(0x0003);
    } else {
      std::ostringstream oss;
      oss << back;
      blk = "\033[48;5;" + oss.str() + "m" + (blk_ == "null" ? "  " : blk_) + "\033[0m";
    }
  }
  void setBack(const unsigned short back) {
    if (back > 255) {
      throw errcode(0x0003);
    } else {
      std::stringstream oss;
      oss << back;
      blk = "\033[48;5;" + oss.str() + "m" + blk + "\033[0m";
    }
  }

  std::string getID() const& {
    return id;
  }
  void setID(const std::string& id_) {
    if (id_.empty()) throw errcode(0x0001);
    else if (!isValidID(id_)) throw errcode(0x0002);
    else id = id_;
  }

  std::string getKit() const& {
    return kit;
  }
  void setKit(const std::string& kit_) {
    if (kit_.empty()) throw errcode(0x0001);
    else if (!isValidID(kit_)) throw errcode(0x0002);
    else kit = kit_;
  }

  std::string getName() const& {
    return name;
  }
  void setName(const std::string& name_) {
    name = name_;
  }

  void fromJson(const json& j) {
    setBlk(j.value("blk", "null"));
    setName(j.at("name").template get<std::string>());
    setID(j.value("id", randomID()));
  }
  json toJson() const& {
    json j {
      {"blk", getBlk()},
      {"id", getID()},
      {"name", getName()}
    };
    return j;
  }
};// class block

class combination {
private:
  std::vector<std::vector<block>> cbnt;
  std::string id;
  std::string kit;
  std::string name;

public:
  combination() {}
  combination(const size_t w, const size_t h) {
  	setW(w);
  	setH(h);
  }
  combination(
    const size_t w,
    const size_t h,
    const std::string& id_,
    const std::string& kit_,
    const std::string& name_
  ) {
    setW(w);
    setH(h);
    setID(id_);
    setKit(kit_);
    setName(name_);
  }
  combination(
  	const size_t w,
  	const size_t h,
  	const std::vector<std::vector<block>>& cbnt_,
  	const std::string& id_,
  	const std::string& kit_,
  	const std::string& name_
  ) {
  	setW(w);
  	setH(h);
  	setCbnt(cbnt_);
  	setID(id_);
  	setKit(kit_);
  	setName(name_);
  }
  combination(const json& j) {
  	fromJson(j);
  }
  combination(const json& j, const std::string& kit_) {
  	fromJson(j);
  	setKit(kit_);
  }

	std::vector<std::vector<block>> getCbnt() const& {
		return cbnt;
	}
	void setCbnt(const std::vector<std::vector<block>>& cbnt_) {
		cbnt = cbnt_;
	}

  block getPos(const size_t r, const size_t c) const& {
    if (r >= cbnt.size() || c >= cbnt[0].size()) throw errcode(0x0005);
    return cbnt[r][c];
  }
  void setPos(const size_t r, const size_t c, const block& blk) {
    if (r >= cbnt.size() || c >= cbnt[0].size()) throw errcode(0x0005);
    cbnt[r][c] = blk;
  }

  size_t getW() const {
    return cbnt.empty() ? 0 : cbnt[0].size();
  }
  void setW(const size_t w) {
    for (auto& row : cbnt) {
      row.resize(w);
    }
  }

  size_t getH() const {
    return cbnt.size();
  }
  void setH(const size_t h) {
    cbnt.resize(h, std::vector<block>(getW()));
  }

  std::string getID() const& {
  	return id;
  }
  void setID(const std::string& id_) {
  	if (id_.empty()) throw errcode(0x0001);
  	else if (!isValidID(id_)) throw errcode(0x0002);
  	else id = id_;
  }

  std::string getKit() const& {
    return kit;
  }
  void setKit(const std::string& kit_) {
    if (kit_.empty()) throw errcode(0x0001);
    else if (!isValidID(kit_)) throw errcode(0x0002);
    else kit = kit_;
  }

  std::string getName() const& {
  	return name;
  }
	void setName(const std::string& name_) {
		name = name_;
	}

  void fromJson(const json& j) {
    if (!j.at("blks").is_array()) throw errcode(0x0006);
    setH(j.at("blks").size());
    for (size_t r = 0; r < j.at("blks").size(); r++) {
      if (!j.at("blks")[r].is_array()) throw errcode(0x0006);
      if (j.at("blks")[r].size() != j.at("blks")[0].size()) throw errcode(0x0006);
      setW(j.at("blks")[r].size());
      for (size_t c = 0; c < j.at("blks")[0].size(); c++) {
        setPos(r, c, block(j.at("blks")[r][c].template get<std::string>()));
      }
    }
  }
  json toJson() const& {
  	std::vector<std::vector<json>> cbnt_json;
  	cbnt_json.reserve(getH());
  	for (size_t r = 0; r < getH(); r++) {
  		cbnt_json[r].reserve(getW());
  		for (size_t c = 0; c < getW(); c++) {
  			cbnt_json[r][c] = cbnt[r][c].toJson();
  		}
  	}
  	json j {
  		{"blks", cbnt_json},
  		{"id", getID()},
  		{"name", getName()}
  	};
  	return j;
  }
};// class combination

class kit {
private:
  std::string author;
  std::vector<block> blks;
  std::vector<combination> cbnts;
  std::string id;
  std::string name;

public:
  kit() {}
  kit(const std::string& path) {
    fromFile(path);
  }

  std::string getAuthor() const& {
    return author;
  }
  void setAuthor(const std::string& author_) {
    author = author_;
  }

  std::vector<block> getBlks() const& {
    return blks;
  }
  void AddToBlks(const block& blk) {
    blks.emplace_back(blk);
  }
  void DelFromBlks(const size_t i) {
    if (i < blks.size()) blks.erase(blks.begin() + i);
    else throw errcode(0x0004);
  }
  void ClearBlks() {
    blks.clear();
  }

  std::vector<combination> getCbnts() const& {
  	return cbnts;
  }
  void AddToCbnts(const combination& cbnt) {
  	cbnts.emplace_back(cbnt);
  }
  void DelFromCbnts(const size_t i) {
  	if (i < cbnts.size()) cbnts.erase(cbnts.begin() + i);
  	else throw errcode(0x0004);
  }
  void ClearCbnts() {
  	cbnts.clear();
  }

  std::string getID() const& {
    return id;
  }
  void setID(const std::string& id_) {
    if (id_.empty()) throw errcode(0x0001);
    else if (!isValidID(id_)) errcode(0x0002);
    else id = id_;
  }

  std::string getName() const& {
    return name;
  }
  void setName(const std::string& name_) {
    name = name_;
  }

  void fromJson(const json& j) {
    setAuthor(j.value("author", "Unknown"));
    setID(j.value("id", randomID()));
    setName(j.value("name", "Empty Name"));
    for (const json& blk : j.at("blks")) {
      AddToBlks(block(blk, id));
    }
    for (const json& cbnt : j.at("cbnts")) {
    	AddToCbnts(combination(cbnt, id));
    }
  }
  json toJson() const& {
    std::vector<json> blks_json;
    std::vector<json> cbnts_json;
    blks_json.reserve(blks.size());
    for (const auto& i : blks) {
      blks_json.emplace_back(i.toJson());
    }
    cbnts_json.reserve(cbnts.size());
    for (const auto& i : cbnts) {
    	cbnts_json.emplace_back(i.toJson());
    }
    json j {
      {"author", getAuthor()},
      {"id", getID()},
      {"name", getName()},
      {"blks", blks_json},
      {"cbnts", cbnts_json}
    };
    return j;
  }

  void fromFile(const std::string& path) {
    std::ifstream ifs(path);
    if (!ifs) throw errcode(strerror(errno));
    json j;
    ifs >> j;
    fromJson(j);
  }
  void toFile(const std::string& path) {
    std::ofstream ofs(path);
    ofs << std::setw(4) << toJson() << std::endl;
  }
}; // class kit

class map {
private:
  std::vector<std::vector<block>> m;

public:
  map() {}
  map(const size_t w, const size_t h) : m(h, std::vector<block>(w)) {}
}; // class map
