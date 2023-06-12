#include "OutSerializer.h"
#include "Galactose/Core/Uuid.h"

#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <fstream>

#define GT_MATH_CONVERT(_T)\
	template<>\
	struct convert<_T> {\
		static Node encode(const _T& a_rhs) {\
			Node node;\
			node.SetStyle(YAML::EmitterStyle::Flow);\
			for (int i = 0; i < _T::length(); ++i)\
				node.push_back(a_rhs[i]);\
			return node;\
		}\
		static bool decode(const Node& a_node, _T& a_rhs) {\
			if (!a_node.IsSequence() || a_node.size() != _T::length())\
				return false;\
			for (int i = 0; i < _T::length(); ++i)\
				a_rhs[i] = a_node[i].as<float>();\
			return true;\
		}\
	}\

namespace YAML {
	template <typename T>
	Emitter& operator<<(Emitter& out, const T& rhs) {
		out << convert<T>::encode(rhs);
		return out;
	}

	GT_MATH_CONVERT(Galactose::Vector2);
	GT_MATH_CONVERT(Galactose::Vector3);
	GT_MATH_CONVERT(Galactose::Vector4);
	GT_MATH_CONVERT(Galactose::Quaternion);

	template<>
	struct convert<Galactose::Uuid> {
		static Node encode(const Galactose::Uuid& a_uuid) { return Node(a_uuid.toHex()); }

		static bool decode(const Node& a_node, Galactose::Uuid& a_uuid) {
			a_uuid = Galactose::Uuid::fromHex(a_node.Scalar());
			return a_uuid.isValid();
		}
	};

	template<>
	struct convert<std::filesystem::path> {
		static Node encode(const std::filesystem::path& a_path) { return Node(a_path); }

		static bool decode(const Node& a_node, std::filesystem::path& a_path) {
			a_path = a_node.Scalar();
			return true;
		}
	};
}

namespace Galactose {
	OutSerializer::OutSerializer() :
		m_emitter(std::make_shared<YAML::Emitter>())
	{}

	void OutSerializer::save(const std::string& a_filePath) {
		std::ofstream fileStream(a_filePath);
		fileStream << m_emitter->c_str();
	}

	OutSerializer& OutSerializer::operator<<(const Manip a_rhs) {
		*m_emitter << static_cast<YAML::EMITTER_MANIP>(a_rhs);
		return *this;
	}

	OutSerializer& OutSerializer::operator<<(const Uuid& a_rhs) {
		*m_emitter << a_rhs;
		return *this;
	}

	OutSerializer& OutSerializer::operator<<(const float a_rhs) {
		*m_emitter << a_rhs;
		return *this;
	}

	OutSerializer& OutSerializer::operator<<(const std::string& a_rhs) {
		*m_emitter << a_rhs;
		return *this;
	}

	OutSerializer& OutSerializer::operator<<(const std::filesystem::path& a_rhs) {
		*m_emitter << a_rhs;
		return *this;
	}

	OutSerializer& OutSerializer::operator<<(const Vector2& a_rhs) {
		*m_emitter << a_rhs;
		return *this;
	}

	OutSerializer& OutSerializer::operator<<(const Vector3& a_rhs) {
		*m_emitter << a_rhs;
		return *this;
	}

	OutSerializer& OutSerializer::operator<<(const Vector4& a_rhs) {
		*m_emitter << a_rhs;
		return *this;
	}

	OutSerializer& OutSerializer::operator<<(const Quaternion& a_rhs) {
		*m_emitter << a_rhs;
		return *this;
	}
}
