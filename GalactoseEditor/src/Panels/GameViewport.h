#pragma once

#include "Viewport.h"
#include "EditorContext.h"

namespace GalactoseEditor {
	class GameViewport : public Viewport {
	public:
		GameViewport(const std::shared_ptr<EditorContext>& a_editorContext) :
			Viewport("Game", a_editorContext)
		{}

		Galactose::Camera* getCamera() const override { return m_editorContext->scene()->mainCamera(); }
		void onEvent(const std::shared_ptr<Galactose::Event>& a_event) override { m_editorContext->scene()->processEvent(a_event); }
	};
}
