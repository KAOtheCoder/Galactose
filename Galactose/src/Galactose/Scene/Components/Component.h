#pragma once

#include "Galactose/Core/Global.h"
#include "Galactose/Scene/Entity.h"

#define GT_COMPONENT_COMMON(C)\
public:\
	GT_UNCOPYABLE(C)\
	GT_UNMOVABLE(C)\
	std::string name() const override { return GT_STRINGIFY(C); }\
	void destroy() override { entity()->removeComponent<C>(); }\
	static uint32_t staticType() { return entt::type_hash<C>::value(); }\
	uint32_t type() const override { return C::staticType(); }\
	static Galactose::Component* create(Galactose::Entity* a_entity) { return a_entity->addComponent<C>(); }\
	static std::string staticName() { return GT_STRINGIFY(C); }\
private:\

#define GT_COMPONENT(C)\
GT_COMPONENT_COMMON(C)\
	inline static Component::MetaComponent<C> s_meta;\

namespace Galactose {
	class Transform;
	class OutSerializer;

	class Component : public SceneObject {
	public:
		Entity* entity() const { return m_entity; }
		Transform* getTransform() const { return m_entity->getTransform(); }

		virtual std::string name() const = 0;
		virtual uint32_t type() const = 0;
		virtual void destroy() = 0;

		void save(OutSerializer& out) const;
		bool load(const NodeSerializer& node);

	protected:
		class MetaBase {
		public:
			static MetaBase* meta(const std::string& name);
			GT_API static const std::set<std::string>& scripts();

			virtual std::string name() const = 0;
			virtual uint32_t type() const = 0;
			virtual Component* create(Galactose::Entity* a_entity) = 0;

		protected:
			GT_API void insert();
			GT_API void insertScript();
			GT_API void remove();

			inline static std::unordered_map<std::string, MetaBase*> s_metasByName;
			inline static std::unordered_map<uint32_t, MetaBase*> s_metasByType;
			inline static std::set<std::string> s_scripts;
		};

		template <class C>
		class MetaComponent : public MetaBase {
		public:
			MetaComponent() { insert(); }

			std::string name() const override { return C::staticName(); }
			uint32_t type() const override { return C::staticType(); }
			Component* create(Galactose::Entity* a_entity) override { return C::create(a_entity); }
		};

		GT_API virtual void start() {}
		GT_API virtual void saveContent(OutSerializer& out) const = 0;
		GT_API virtual bool loadContent(const NodeSerializer& node) = 0;

	private:
		Entity* m_entity = nullptr;

		friend class Entity;
	};
}
