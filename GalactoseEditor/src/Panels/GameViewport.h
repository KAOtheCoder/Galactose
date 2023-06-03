#pragma once

#include "Viewport.h"
#include "EditorContext.h"

namespace GalactoseEditor {
	class GameViewport : public Viewport {
	public:
		GameViewport(const std::shared_ptr<EditorContext>& a_sceneContext) :
			Viewport("Game", a_sceneContext)
		{}

		Galactose::Camera* getCamera() const override { return m_sceneContext->scene()->mainCamera(); }
		void onEvent(const std::shared_ptr<Galactose::Event>& a_event) override { m_sceneContext->scene()->processEvent(a_event); }
	};
}
