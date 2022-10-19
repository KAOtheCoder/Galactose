#pragma once

#include "GalactosePCH.h"
#include "Entity.h"
#include "Scene.h"

namespace Galactose {
	Entity::Entity(Scene* a_scene) :
		m_id(a_scene->m_registry.create()),
		m_scene(a_scene)
	{}
}