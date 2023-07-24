#pragma once

#include <Galactose/Scene/Components/Script.h>
#include <Galactose/Math/Math.h>

namespace GalactoseEditor {
	class CameraController : public Galactose::Script {
		GT_COMPONENT(GalactoseEditor::CameraController)

	public:
		CameraController() = default;

		void onEvent(const std::shared_ptr<Galactose::Event>& event) override;

	private:
		bool m_rotate = false;
		Galactose::Vector2 m_cursorPos;
		Galactose::Quaternion m_rotation;
	};
}
