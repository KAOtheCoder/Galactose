#pragma once

#include "Galactose/Core/Global.h"
#include "Galactose/Scene/Entity.h"

#define GT_COMPONENT_COMMON(C)\
public:\
	GT_UNMOVABLE(C)\
	std::string name() const override { return GT_STRINGIFY(C); }\
	void destroy() override { entity()->removeComponent<C>(); }\
	static uint32_t staticType() { return entt::type_hash<C>::value(); }\
	uint32_t type() const override { return C::staticType(); }\

#define GT_COMPONENT(C)\
public:\
	GT_UNMOVABLE(C)\
	std::string name() const override { return GT_STRINGIFY(C); }\
	void destroy() override { entity()->removeComponent<C>(); }\
	static uint32_t staticType() { return entt::type_hash<C>::value(); }\
	uint32_t type() const override { return C::staticType(); }\
	static Galactose::Component* create(Galactose::Entity* a_entity) { return a_entity->addComponent<C>(); }\
	static std::string staticName() { return GT_STRINGIFY(C); }\
private:\
	inline static Component::Meta<C> s_meta;\

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
		class MetaBase {
		public:
			static MetaBase* meta(const std::string& name);
			
			virtual std::string name() const = 0;
			virtual uint32_t type() const = 0;
			virtual Component* create(Galactose::Entity* a_entity) = 0;

		protected:
			GT_API void insert();

			inline static std::unordered_map<std::string, MetaBase*> s_metasByName;
			inline static std::unordered_map<uint32_t, MetaBase*> s_metasByType;
		};

		template <class C>
		class Meta : public MetaBase {
		public:
			Meta() { insert(); }

			std::string name() const override { return C::staticName(); }
			uint32_t type() const override { return C::staticType(); }
			Component* create(Galactose::Entity* a_entity) override { return C::create(a_entity); }
		};

		GT_API virtual void start() {}
		GT_API virtual void saveContent(YAML::Emitter& out) const = 0;
		GT_API virtual bool loadContent(const YAML::Node& node) = 0;

	private:
		Entity* m_entity = nullptr;

		friend class Entity;
	};
}
