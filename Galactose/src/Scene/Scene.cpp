#include "GalactosePCH.h"
#include "Scene.h"
#include "Entity.h"

namespace Galactose {
	Scene::Scene(const std::string& a_name) :
		m_name(a_name)
	{}

	std::shared_ptr<Entity> Scene::addEntity() {
		return std::make_shared<Entity>(this);
	}
}
