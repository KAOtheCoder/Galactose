#pragma once

#include "Math.h"

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
				a_rhs.x = a_node[i].as<float>();\
			return true;\
		}\
	}\

namespace YAML {
	GT_MATH_CONVERT(Galactose::Vector2);
	GT_MATH_CONVERT(Galactose::Vector3);
	GT_MATH_CONVERT(Galactose::Vector4);
	GT_MATH_CONVERT(Galactose::Quaternion);

	template <typename T>
	Emitter& operator<<(Emitter& out, const T& rhs) {
		out << convert<T>::encode(rhs);
		return out;
	}
}
