#pragma once

#include "Panels/Panel.h"
#include "EditorSceneData.h"

#include <memory>
#include <unordered_map>

#include <Math/Math.h>

namespace Galactose {
	class Texture;
	class Transform;
	class SpriteRenderer;
	class Camera;
}

namespace GalactoseEditor {
	class Inspector : public Panel {
	public:
		Inspector(const std::shared_ptr<EditorSceneData>& sceneData);

		void onUpdate() override;

	private:
		static void drawLabel(const char* label);
		static bool dragVector3Axis(const int axis, float& value);
		static bool dragVector(const char* label, const int axisCount, float* value);
		static bool colorButton(const char* descId, Galactose::Vector4& color);
		bool drawFileInput(const char* label, std::string& path, const std::string& emptyText = "");
		bool iconButton(const char* icon);

		static bool drawComponentHeader(const char* label);

		template <class C>
		C* getSelectedComponent() const { return m_sceneData->selectedEntity()->getComponent<C>(); }

		template <class C>
		void bindComponentDrawer() { m_componentDrawers[entt::type_hash<C>::value()] = &Inspector::drawComponent<C>; }

		template <class C>
		void drawComponent();

		std::shared_ptr<EditorSceneData> m_sceneData;
		std::unordered_map<std::string, std::shared_ptr<Galactose::Texture>> m_icons;
		std::unordered_map<uint32_t, void(Inspector::*)()> m_componentDrawers;
	};

	extern template void Inspector::drawComponent<Galactose::Transform>();
	extern template void Inspector::drawComponent<Galactose::SpriteRenderer>();
	extern template void Inspector::drawComponent<Galactose::Camera>();
}
