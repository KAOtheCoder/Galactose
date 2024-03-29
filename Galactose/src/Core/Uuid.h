#pragma once

#include <unordered_set>
#include <string>

namespace Galactose {
	class Uuid {
	public:
		static Uuid create();
		static Uuid fromHex(const std::string& hex);

		Uuid(const uint64_t a_id = 0);

		bool isValid() const { return m_id != 0 && m_id != ULLONG_MAX; }
		std::string toHex() const;

		bool operator==(const Uuid& a_other) const { return m_id == a_other.m_id; }

	private:
		inline static std::unordered_set<uint64_t> s_ids;

		uint64_t m_id;

		friend struct std::hash<Galactose::Uuid>;
	};
}

template<>
struct std::hash<Galactose::Uuid> {
	std::size_t operator()(Galactose::Uuid const& uuid) const noexcept {
		return std::hash<uint64_t>{}(uuid.m_id);
	}
};
