#pragma once

#include "Viewport.h"
#include "EditorSceneData.h"

namespace GalactoseEditor {
	class GameViewport : public Viewport {
	public:
		GameViewport::GameViewport(const std::shared_ptr<EditorSceneData>& a_sceneData) :
			Viewport("Game", a_sceneData)
		{}

		Galactose::Camera* getCamera() const override { return m_sceneData->scene()->mainCamera(); }
		void onEvent(const std::shared_ptr<Galactose::Event>& a_event) override { m_sceneData->scene()->processEvent(a_event); }
	};
}
