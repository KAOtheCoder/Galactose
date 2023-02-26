#pragma once

#include "Core/Global.h"
#include "Scene/Entity.h"

#define GT_COMPONENT(_C)\
public:\
	std::string name() const override { return GT_STRINGIFY(_C); }\
private:\
	inline static Component::Meta s_meta{ GT_STRINGIFY(_C), [](Entity* a_entity) { return a_entity->addComponent<_C>(); } };\

namespace YAML {
	class Emitter;
}

namespace Galactose {
	class Component : public SceneObject {
	public:
		Entity* entity() const { return m_entity; }

		virtual std::string name() const = 0;
		void save(YAML::Emitter& out) const;

	protected:
		struct Meta {
			Meta(const std::string& a_name, const std::function<Component* (Entity* a_entity)>& a_creator);

			inline static std::unordered_map<std::string, Meta*> s_metas;

			const std::function<Component*(Entity*)> creator;
		};

		virtual void saveContent(YAML::Emitter& out) const = 0;

	private:
		Entity* m_entity;

		friend class Entity;
	};
}
