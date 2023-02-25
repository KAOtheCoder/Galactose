#pragma once

#include "Scene/Entity.h"

namespace YAML {
	class Emitter;
}

namespace Galactose {
	class Component : public SceneObject {
	public:
		Entity* entity() const { return m_entity; }

		virtual void save(YAML::Emitter& out) const = 0;

	protected:
		static void beginSave(YAML::Emitter& out, const char* name);
		static void endSave(YAML::Emitter& out);

	private:
		Entity* m_entity;

		friend class Entity;
	};
}
