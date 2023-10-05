#include "Script.h"
#include "Galactose/Serialization/OutSerializer.h"
#include "Galactose/Serialization/NodeSerializer.h"

#define GT_PROPERTY_TEMPLATE_CALL_IMP(a_property, a_func, a_data)\
	switch (a_property->type()) {\
		case DataType::Bool: a_func<bool>(a_property, a_data);\
			break;\
		case DataType::Float: a_func<float>(a_property, a_data);\
			break;\
		case DataType::Vector2: a_func<Vector2>(a_property, a_data);\
			break;\
		case DataType::Vector3: a_func<Vector3>(a_property, a_data);\
			break;\
		case DataType::Vector4: a_func<Vector4>(a_property, a_data);\
			break;\
	}\

namespace Galactose {
	template <typename T>
	void saveProperty(PropertyBase* a_property, void* a_data) {
		*static_cast<OutSerializer*>(a_data) << static_cast<AccessibleProperty<T>*>(a_property)->get();
	}

	void Script::saveContent(OutSerializer& a_out) const {
		for (auto property : m_properties) {
			if (property->isReadOnly())
				continue;

			if (property->type() == DataType::Void) {
				std::cerr << "Ignoring to save property '" << property->name() << "' due to unsupported type." << std::endl;
			}
			else {
				a_out << OutSerializer::Key << property->name() << OutSerializer::Value;

				void* out = &a_out;
				GT_PROPERTY_TEMPLATE_CALL_IMP(property, saveProperty, out);
			}
		}
	}

	template <typename T>
	void loadProperty(PropertyBase* a_property, const void* a_data) {
		static_cast<AccessibleProperty<T>*>(a_property)->set((*static_cast<const NodeSerializer*>(a_data)).as<T>());
	}

	bool Script::loadContent(const NodeSerializer& a_node) {
		for (auto property : m_properties) {
			if (property->isReadOnly())
				continue;

			const auto& node = a_node[property->name()];
			if (node.isNull())
				continue;

			if (property->type() == DataType::Void) {
				std::cerr << "Ignoring to load property '" << property->name() << "' due to unsupported type." << std::endl;
			}
			else {
				const void* node = &a_node;
				GT_PROPERTY_TEMPLATE_CALL_IMP(property, loadProperty, node);
			}
		}

		return true;
	}
}
