#pragma once

#include "GalactosePCH.h"
#include "Entity.h"
#include "Scene.h"

namespace Galactose {
	Entity* Entity::create(Scene* a_scene) {
		const auto id = a_scene->m_registry.create();
		auto& entity = a_scene->m_registry.emplace<Entity>(id);
		entity.m_data = { a_scene, id };

		return &entity;
	}
}
