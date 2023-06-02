#pragma once

#include <Scene/Components/Script.h>
#include <Math/Math.h>

namespace GalactoseEditor {
	class CameraController : public Galactose::Script {
		GT_PRIVATE_COMPONENT(GalactoseEditor::CameraController)

	public:
		CameraController() = default;

		void onEvent(const std::shared_ptr<Galactose::Event>& event) override;

	private:
		Galactose::Vector2 m_cursorPos;
	};
}
