#pragma once

#include "Panels/SceneViewport.h"
#include "Panels/SceneHierarchy.h"
#include "Panels/Inspector.h"

#include <Core/Layer.h>
#include <Math/Math.h>
#include <Renderer/Camera.h>
#include <Renderer/Texture.h>

namespace Galactose {
	class Window;
	class Framebuffer;
}

namespace GalactoseEditor {
	class EditorSceneData;

	class EditorLayer : public Galactose::Layer {
	public:
		EditorLayer(Galactose::Window* window);
		~EditorLayer() override;

		void onUpdate() override;
		void onEvent(const std::shared_ptr<Galactose::Event>& a_event) override;

	private:
		void drawMenuBar();

		std::shared_ptr<EditorSceneData> m_sceneData;
		SceneViewport m_sceneViewport;
		SceneHierarchy m_sceneHierarchy;
		Inspector m_inspector;
	};
}
