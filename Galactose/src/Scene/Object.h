#pragma once
#include "Scene.h"

namespace Galactose {
	class Object {
	protected:
		struct Data {
			Scene* scene = nullptr;
			entt::entity entityId = entt::null;
		};

	public:
		template <class O>
		class Ptr {
		public:
			Ptr() = default;

			Ptr(const O* a_object) : m_data(static_cast<const Object*>(a_object)->m_data) {
				static_assert(std::is_base_of<Object, O>::value, "Pointer must inherit from Object.");
			}

			bool isValid() const { 
				if (!m_data.scene)
					return false;

				auto& registry = m_data.scene->m_registry;
				return registry.valid(m_data.entityId) && registry.any_of<O>(m_data.entityId); 
			}

			O* get() const { return &(m_data.scene->m_registry.get<O>(m_data.entityId));}
			O* operator->() const { return get(); }

		private:
			Data m_data;
		};

		Object() = default;
		virtual ~Object() = default;

	protected:
		Object(const Data& a_data) : m_data(a_data) {}

		Data m_data;

		friend class Entity;
	};
}
