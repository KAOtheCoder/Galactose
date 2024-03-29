#pragma once

#include "Core/Global.h"
#include "Scene/Entity.h"

#define GT_COMPONENT_COMMON(C)\
public:\
	GT_UNMOVABLE(C)\
	std::string name() const override { return GT_STRINGIFY(C); }\
	void destroy() override { entity()->removeComponent<C>(); }\

#define GT_COMPONENT(C)\
GT_COMPONENT_COMMON(C)\
	static Galactose::Component* create(Galactose::Entity* a_entity) { return a_entity->addComponent<C>(); }\
	static std::string staticName() { return GT_STRINGIFY(C); }\
	static uint32_t staticType() { return s_meta.id; }\
	uint32_t type() const override { return s_meta.id; }\
private:\
	inline static Component::Meta s_meta{ GT_STRINGIFY(C), entt::type_hash<C>::value(), &C::create };\

#define GT_PRIVATE_COMPONENT(C)\
GT_COMPONENT_COMMON(C)\
	uint32_t type() const override { return entt::type_hash<C>::value(); }\

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
		virtual uint32_t type() const = 0;
		virtual void destroy() = 0;

		void save(YAML::Emitter& out) const;
		bool load(const YAML::Node& node);

	protected:
		struct Meta {
			Meta(const std::string& name, const uint32_t id, Component*(*creator)(Entity*));

			static Meta* meta(const std::string& name);
			inline static std::unordered_map<std::string, Meta*> s_metas;
			
			const uint32_t id;
			Component*(*creator)(Entity*);
		};

		virtual void start() {}
		virtual void saveContent(YAML::Emitter& out) const = 0;
		virtual bool loadContent(const YAML::Node& node) = 0;

	private:
		Entity* m_entity = nullptr;

		friend class Entity;
	};
}
