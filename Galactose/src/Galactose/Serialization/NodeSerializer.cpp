#include "NodeSerializer.h"
#include "Convert.h"

#define GT_AS_IMP(T) T NodeSerializer::as(const T& a_fallback) const { return m_node->as<T>(a_fallback); }

namespace Galactose {
	NodeSerializer::Iterator& NodeSerializer::Iterator::operator++() {
		m_iterator->operator++();
		return *this;
	}

	NodeSerializer::Iterator NodeSerializer::Iterator::operator++(int) {
		return NodeSerializer::Iterator(std::make_shared<YAML::iterator>(m_iterator->operator++()));
	}

	NodeSerializer NodeSerializer::Iterator::operator*() const {
		YAML::Node node = m_iterator->operator*();
		return NodeSerializer(std::make_shared<YAML::Node>(node));
	}

	NodeSerializer NodeSerializer::Iterator::key() const {
		return NodeSerializer(std::make_shared<YAML::Node>(m_iterator->operator*().first));
	}

	NodeSerializer NodeSerializer::Iterator::value() const {
		return NodeSerializer(std::make_shared<YAML::Node>(m_iterator->operator*().second));
	}

	bool NodeSerializer::Iterator::operator!=(const Iterator& rhs) const {
		return *m_iterator != *rhs.m_iterator;
	}

	NodeSerializer::NodeSerializer() :
		m_node(std::make_shared<YAML::Node>())
	{}

	NodeSerializer::NodeSerializer(const NodeSerializer& other) :
		m_node(std::make_shared<YAML::Node>(*other.m_node))
	{}

	NodeSerializer& NodeSerializer::operator=(const NodeSerializer& other) {
		if (&other != this)
			m_node = std::make_shared<YAML::Node>(*other.m_node);

		return *this;
	}

	NodeSerializer NodeSerializer::loadFile(const std::string& a_filePath) {
		NodeSerializer node;
		try
		{
			node.m_node = std::make_shared<YAML::Node>(YAML::LoadFile(a_filePath));
		}
		catch (const YAML::Exception& e)
		{
			std::cerr << "Failed to load file '" << a_filePath << "': " << e.what() << std::endl;
		}

		return node;
	}

	bool NodeSerializer::isNull() const {
		return m_node->IsNull();
	}

	bool NodeSerializer::isMap() const {
		return m_node->IsMap();
	}

	NodeSerializer NodeSerializer::operator[](const std::string& key) const {
		return NodeSerializer(std::make_shared<YAML::Node>(m_node->operator[](key)));
	}

	NodeSerializer::Iterator NodeSerializer::begin() {
		return NodeSerializer::Iterator(std::make_shared<YAML::iterator>(m_node->begin()));
	}

	NodeSerializer::Iterator NodeSerializer::begin() const {
		return NodeSerializer::Iterator(std::make_shared<YAML::iterator>(m_node->begin()));
	}

	NodeSerializer::Iterator NodeSerializer::end() {
		return NodeSerializer::Iterator(std::make_shared<YAML::iterator>(m_node->end()));
	}

	NodeSerializer::Iterator NodeSerializer::end() const {
		return NodeSerializer::Iterator(std::make_shared<YAML::iterator>(m_node->end()));
	}

	GT_AS_IMP(float)
	GT_AS_IMP(std::string)
	GT_AS_IMP(std::filesystem::path)
	GT_AS_IMP(Uuid)
	GT_AS_IMP(Vector2)
	GT_AS_IMP(Vector3)
	GT_AS_IMP(Vector4)
	GT_AS_IMP(Quaternion)
}
