#pragma once

#include <cstddef>
#include "err.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>
#include <vector>

using json = nlohmann::json;

namespace block {
	class block {
	private:
		std::string blk;
		std::string id;
		std::string kit;
		std::string name;

	public:
		block() {setBlk("null");}
		block(const unsigned short back) {setBlk("null", back);}
		block(const std::string& blk_) {setBlk(blk_);}
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
		block(json j) {import_json(j);}
		block(json j, std::string kit_) {
			import_json(j);
			setKit(kit_);
		}
	
		std::string getBlk() const& {return blk;}
		void setBlk(const std::string& blk_) {blk = (blk_ == "null" ? "  " : blk_);}
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

		std::string getID() const& {return id;}
		void setID(const std::string& id_) {
  		if (id_.empty()) throw errcode(0x0001);
 			else if (!isValidID(id_)) throw errcode(0x0002);
			else id = id_;
		}

		std::string getKit() const& {return kit;}
		void setKit(const std::string& kit_) {
			if (kit_.empty()) throw errcode(0x0001);
			else if (!isValidID(kit_)) throw errcode(0x0002);
			else kit = kit_;
		}

		std::string getName() const& {return name;}
		void setName(const std::string& name_) {name = name_;}

		void import_json(const json& j) {
			setBlk(j.at("blk"));
			setName(j.at("name"));
			setID(j.at("id"));
		}
		json export_json() const& {
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

	public:
		combination(const size_t w, const size_t h) : cbnt(h, std::vector<block>(w)) {}

		block getPos(const size_t x, const size_t y) const& {
			if (y >= cbnt.size() || x >= cbnt[y].size()) throw errcode(0x0005);
			return cbnt[y][x];
		}
		void setPos(const size_t x, const size_t y, const block& blk) {
			if (y >= cbnt.size() || x >= cbnt[y].size()) throw errcode(0x0005);
			cbnt[y][x] = blk;
		}

		size_t getW() const {
			return cbnt.empty() ? 0 : cbnt[0].size();
		}
		void setW(const size_t w) {
			for (auto& row : cbnt) {
				row.resize(w);
			}
		}

		size_t getH() const {return cbnt.size();}
		void setH(const size_t h) {
			cbnt.resize(h, std::vector<block>(getW()));
		}
	};// class combination

	class kit {
	private:
		std::string author;
		std::vector<block> blks;
		std::string id;
		std::string name;

	public:
		kit() {}
		kit(const json& j) {import_json(j);}
		kit(const std::string& path) {import_file(path);}

		std::string getAuthor() const& {return author;}
		void setAuthor(const std::string& author_) {author = author_;}

		std::vector<block> getBlks() const& {return blks;}
		void AddToBlks(const block& blk) {blks.push_back(blk);}
		void DelFromBlks(const size_t& i) {
			if (i >= 0 && i < blks.size()) blks.erase(blks.begin() + i);
			else throw errcode(0x0004);
		}
		void ClearBlks() {blks.clear();}

		std::string getID() const& {return id;}
		void setID(const std::string& id_) {
			if (id_.empty()) throw errcode(0x0001);
			else if (!isValidID(id_)) errcode(0x0002);
			else id = id_;
		}

		std::string getName() const& {return name;}
		void setName(const std::string& name_) {name = name_;}

		void import_json(const json& j) {
			setAuthor(j.at("author"));
			setID(j.at("id"));
			setName(j.at("name"));
			for (json blk : j.at("blks")) {
				AddToBlks(block(blk, id));
			}
		}
		json export_json() const& {
			std::vector<json> blks_json;
			blks_json.reserve(blks.size());
			for (auto& i : blks) {
				blks_json.push_back(i.export_json());
			}
			json j {
				{"author", getAuthor()},
				{"id", getID()},
				{"name", getName()},
				{"blks", blks_json}
			};
			return j;
		}

		void import_file(const std::string& path) {
			std::ifstream ifs(path);
			if (!ifs) throw errcode(strerror(errno));
			json j;
			ifs >> j;
			import_json(j);
		}
		void export_file(const std::string& path) {
			std::ofstream ofs(path);
			ofs << std::setw(4) << export_json() << std::endl;
		}
	}; // class kit
} // namespace block
