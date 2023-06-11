#pragma once

#include "Galactose/Math/Math.h"
#include "Galactose/Core/Uuid.h"

#include <yaml-cpp/yaml.h>

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
}

namespace Galactose {
	namespace Serialize {
		template <typename T>
		inline T getValue(const YAML::Node& a_node, const std::string& a_key, const T& a_default) {
			const auto& node = a_node[a_key];
			if (node)
				return node.as<T>();
			

			std::cerr << "Invalid key: " << a_key << std::endl;
			return a_default;
		}
	}
}

#undef GT_MATH_CONVERT
