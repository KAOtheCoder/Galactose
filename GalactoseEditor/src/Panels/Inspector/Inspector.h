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
		struct ComponentInfo {
			std::string name;
			void(Inspector::*draw)(Galactose::Component*);
			bool(Galactose::Entity::*has)() const;
			Galactose::Component*(*create)(Galactose::Entity*);
		};

		static std::string toReadableName(const std::string& name);
		static void drawLabel(const char* label);
		static bool dragVector3Axis(const int axis, float& value);
		static bool dragVector(const char* label, const int axisCount, float* values);
		static bool checkBox(const char* label, bool& value);
		static bool dragFloat(const char* label, float& value, const float speed = 0.1f, const float min = 0, const float max = 0);
		static bool spanDragFloat(const char* label, float& value, const float speed = 0.1f, const float min = 0, const float max = 0);
		static bool colorButton(const char* descId, Galactose::Vector4& color);
		static void openPopup(const char* label);

		bool drawFileInput(const char* label, std::string& path, const std::string& emptyText = "");
		bool iconButton(const char* icon);

		bool drawComponentHeader(Galactose::Component* component, const char* a_title);
		void drawComponent(Galactose::Component* component);
		void drawScript(Galactose::Script* script);

		template <class C>
		C* getSelectedComponent() const { return m_sceneData->selectedEntity()->getComponent<C>(); }

		template <class C>
		void bindComponent() { 
			m_componentInfos[C::staticType()] = { 
				toReadableName(C::staticName()), 
				&Inspector::drawComponentContentHelper<C>,
				&Galactose::Entity::hasComponent<C>,
				&C::create
			}; 
		}

		template <class C>
		void drawComponentContent(C* component);

		template <class C>
		void drawComponentContentHelper(Galactose::Component* a_component) {
			drawComponentContent<C>(static_cast<C*>(a_component));
		}

		std::shared_ptr<EditorSceneData> m_sceneData;
		std::unordered_map<std::string, std::shared_ptr<Galactose::Texture>> m_icons;
		std::unordered_map<uint32_t, ComponentInfo> m_componentInfos;
		Galactose::Component* m_removeComponent = nullptr;
	};

	extern template void Inspector::drawComponentContent<Galactose::Transform>(Galactose::Transform*);
	extern template void Inspector::drawComponentContent<Galactose::SpriteRenderer>(Galactose::SpriteRenderer*);
	extern template void Inspector::drawComponentContent<Galactose::Camera>(Galactose::Camera*);
}
