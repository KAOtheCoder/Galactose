#include "Uuid.h"

#include <random>
#include <sstream>
#include <iomanip>
#include <iostream>

namespace Galactose {
	Uuid::Uuid(const uint64_t a_id) : 
		m_id(a_id) 
	{
		s_ids.insert(m_id);
	}

	Uuid Uuid::create() {
		static std::random_device s_randomDevice;
		static std::mt19937_64 s_generator(s_randomDevice());
		static std::uniform_int_distribution<uint64_t> s_distribution(uint64_t(1), ULLONG_MAX - 1);

		auto id = s_distribution(s_generator);

		while (s_ids.find(id) != s_ids.end())
			id = s_distribution(s_generator);

		return Uuid(id);
	}

	Uuid Uuid::fromHex(const std::string& a_hex) {
		const uint64_t id = strtoull(a_hex.c_str(), nullptr, 16);

		if (id == 0)
			std::cerr << "Failed to convert '" << a_hex << "' to uuid." << std::endl;
		else if (id == ULLONG_MAX)
			std::cerr << "uuid: '" << a_hex << "' is out of range." << std::endl;

		return Uuid(id);
	}

	std::string Uuid::toHex() const {
		std::ostringstream out;
		out << std::setw(16) << std::setfill('0') << std::hex << m_id;

		return out.str();
	}
}
