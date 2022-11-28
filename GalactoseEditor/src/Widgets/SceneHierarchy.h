#include "GalactosePCH.h"

namespace Galactose {
	class Scene;
	class Entity;
}

namespace GalactoseEditor {
	class SceneHierarchy {
	public:
		SceneHierarchy(const std::shared_ptr<Galactose::Scene>& scene);

		void onUpdate();
		void setVisible(const bool a_visible) { m_visible = a_visible; }

	private:
		void drawEntityNode(const Galactose::Entity* entity);

		bool m_visible = true;
		std::weak_ptr<Galactose::Scene> m_scene;
	};
}
