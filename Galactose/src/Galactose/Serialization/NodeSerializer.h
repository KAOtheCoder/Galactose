#pragma once

#include "Galactose/Math/Math.h"

#include <memory>

namespace YAML {
	class Node;

	namespace detail {
		template <typename T>
		class iterator_base;
		struct iterator_value;
	}
}

namespace Galactose {
	class Uuid;

	class NodeSerializer {
	private:
		class Iterator {
		public:
			using iterator = YAML::detail::iterator_base<YAML::detail::iterator_value>;

			Iterator(const std::shared_ptr<iterator>& a_iterator) : m_iterator(a_iterator) {}
			NodeSerializer operator*() const;
			NodeSerializer key() const;
			NodeSerializer value() const;

			bool operator!=(const Iterator& rhs) const;
			Iterator& operator++();
			Iterator operator++(int);

		private:
			std::shared_ptr<iterator> m_iterator;
		};

	public:
		GT_API static NodeSerializer loadFile(const std::string& filePath);

		NodeSerializer(const NodeSerializer& other);
		NodeSerializer& operator=(const NodeSerializer& other);

		GT_API bool isNull() const;
		bool isMap() const;
		GT_API NodeSerializer operator[](const std::string& key) const;

		Iterator begin();
		Iterator begin() const;
		Iterator end();
		Iterator end() const;

		template <typename T>
		T as() const { return as(T()); }

		float as(const float& fallback) const;
		std::string as(const std::string& fallback) const;
		Uuid as(const Uuid& fallback) const;
		Vector2 as(const Vector2& fallback) const;
		Vector3 as(const Vector3& fallback) const;
		Vector4 as(const Vector4& fallback) const;
		Quaternion as(const Quaternion& fallback) const;

	private:
		NodeSerializer();
		NodeSerializer(const std::shared_ptr<YAML::Node>& node) : m_node(node) {}

		std::shared_ptr<YAML::Node> m_node;
	};
}
