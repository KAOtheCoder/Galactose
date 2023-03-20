#pragma once

#include "Core/Global.h"
#include "Scene/Entity.h"

#define GT_COMPONENT(_C)\
public:\
	std::string name() const override { return GT_STRINGIFY(_C); }\
	entt::id_type type() const override { return s_meta.id; }\
private:\
	inline static Component::Meta s_meta{ GT_STRINGIFY(_C), entt::type_hash<_C>::value(), [](Entity* a_entity) { return a_entity->addComponent<_C>(); } };\

namespace YAML {
	class Emitter;
}

namespace Galactose {
	class Transform;

	class Component : public SceneObject {
	public:
		Entity* entity() const { return m_entity; }
		Transform* getTransform() const { return m_entity->getTransform(); }

		virtual std::string name() const = 0;
		virtual entt::id_type type() const = 0;
		void save(YAML::Emitter& out) const;
		bool load(const YAML::Node& node);

	protected:
		struct Meta {
			Meta(const std::string& name, const entt::id_type id, const std::function<Component* (Entity* a_entity)>& creator);

			static Meta* meta(const std::string& name);
			inline static std::unordered_map<std::string, Meta*> s_metas;
			
			const entt::id_type id;
			const std::function<Component*(Entity*)> creator;
		};

		virtual void saveContent(YAML::Emitter& out) const = 0;
		virtual bool loadContent(const YAML::Node& node) = 0;

	private:
		Entity* m_entity = nullptr;

		friend class Entity;
	};
}
