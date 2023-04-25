#pragma once

#include <Math/Math.h>

#include <string>
#include <memory>

namespace Galactose {
	class Event;
}

namespace GalactoseEditor {
	class Panel {
	public:
		static Panel* focusedPanel() { return s_focusedPanel; }

		Panel(const std::string& a_name);
		virtual ~Panel();

		std::string name() const { return m_name; }
		void setVisible(const bool a_visible) { m_visible = a_visible; }

		Galactose::Vector2 padding() const;
		void setPadding(const Galactose::Vector2& a_padding) { m_padding = a_padding; }

		void update();
		virtual void onUpdate() {}
		virtual void onFocusOut() {}
		virtual void onEvent(const std::shared_ptr<Galactose::Event>& a_event) {}

	private:
		static Panel* s_focusedPanel;

		bool m_visible = true;
		std::string m_name;
		Galactose::Vector2 m_padding;
	};
}
