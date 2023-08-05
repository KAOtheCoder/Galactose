#pragma once

#include "Panels/Panel.h"
#include "EditorContext.h"

#include <memory>
#include <unordered_map>

#include <Galactose/Math/Math.h>

namespace Galactose {
	class PropertyBase;
	class Texture;
	class Component;
	class Transform;
	class SpriteRenderer;
	class Camera;
	class Script;
}

namespace GalactoseEditor {
	class EditorContext;

	class Inspector : public Panel {
	public:
		Inspector(const std::shared_ptr<EditorContext>& sceneContext);

		void onUpdate() override;

	private:
		struct ComponentInfo {
			std::string name;
			void(Inspector::*draw)(Galactose::Component*) = nullptr;
			bool(Galactose::Entity::*has)() const = nullptr;
			Galactose::Component*(*create)(Galactose::Entity*) = nullptr;
		};

		static std::string toReadableName(const std::string& name);
		static void drawLabel(const char* label);
		static bool dragVectorAxis(const int axis, float& value, const Galactose::Vector4& color = { 0.f, 0.f, 0.f, 0.f }, const Galactose::Vector4& hoverColor = { 1.f, 1.f, 1.f, 0.1f });
		static bool dragVector(const char* label, const int axisCount, float* values, const bool colored = false);
		static bool checkBox(const char* label, bool& value);
		static bool dragFloat(const char* label, float& value, const float speed = 0.1f, const float min = 0, const float max = 0);
		static bool spanDragFloat(const char* label, float& value, const float speed = 0.1f, const float min = 0, const float max = 0);
		static bool colorButton(const char* descId, Galactose::Vector4& color);
		static void openPopup(const char* label);
		
		template <typename T>
		static void drawVectorProperty(Galactose::PropertyBase* property);

		bool drawFileInput(const char* label, std::string& path, const std::string& emptyText = "");
		bool iconButton(const char* icon);

		bool drawComponentHeader(Galactose::Component* component, const char* a_title);
		void drawComponent(Galactose::Component* component);
		void drawScript(Galactose::Script* script);

		template <class C>
		void bindComponent();

		template <class C>
		void drawComponentContent(C* component);

		template <class C>
		void drawComponentContentHelper(Galactose::Component* a_component);

		std::shared_ptr<EditorContext> m_sceneContext;
		std::unordered_map<std::string, std::shared_ptr<Galactose::Texture>> m_icons;
		std::unordered_map<uint32_t, ComponentInfo> m_componentInfos;
		Galactose::Component* m_removeComponent = nullptr;
		Galactose::Vector3 m_eulerAngles;
	};

	extern template void Inspector::drawComponentContent<Galactose::Transform>(Galactose::Transform*);
	extern template void Inspector::drawComponentContent<Galactose::SpriteRenderer>(Galactose::SpriteRenderer*);
	extern template void Inspector::drawComponentContent<Galactose::Camera>(Galactose::Camera*);
}
