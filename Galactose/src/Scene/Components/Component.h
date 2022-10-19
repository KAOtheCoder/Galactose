#pragma once

namespace Galactose {
	class Entity;

	class Component {
	public:
		Entity* entity() const { return m_entity; }

	protected:
		Component(Entity* entity) : m_entity(entity) {}

	private:
		Entity* m_entity;
	};
}
