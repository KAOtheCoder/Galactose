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
		
		template <typename T>
		static void drawVectorProperty(Galactose::PropertyBase* property);

		bool drawComponentHeader(Galactose::Component* component, const char* a_title);
		void drawComponent(Galactose::Component* component);
		void drawScript(Galactose::Script* script);

		template <class C>
		void bindComponent();

		template <class C>
		void drawComponentContent(C* component);

		template <class C>
		void drawComponentContentHelper(Galactose::Component* a_component);

		std::shared_ptr<EditorContext> m_editorContext;
		std::unordered_map<uint32_t, ComponentInfo> m_componentInfos;
		Galactose::Component* m_removeComponent = nullptr;
		Galactose::Vector3 m_eulerAngles;
	};

	extern template void Inspector::drawComponentContent<Galactose::Transform>(Galactose::Transform*);
	extern template void Inspector::drawComponentContent<Galactose::SpriteRenderer>(Galactose::SpriteRenderer*);
	extern template void Inspector::drawComponentContent<Galactose::Camera>(Galactose::Camera*);
}
