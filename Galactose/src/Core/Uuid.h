#pragma once

#include <unordered_set>
#include <string>

namespace Galactose {
	class Uuid {
	public:
		static Uuid create();
		static Uuid fromHex(const std::string& hex);

		Uuid(const uint64_t a_id);

		bool isValid() const { return m_id != 0 && m_id != ULLONG_MAX; }
		std::string toHex() const;

	private:
		inline static std::unordered_set<uint64_t> s_ids;

		uint64_t m_id;
	};
}
