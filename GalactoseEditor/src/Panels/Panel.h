#pragma once

#include <Math/Math.h>

#include <string>

namespace GalactoseEditor {
	class Panel {
	public:
		static Panel* focusedPanel() { return s_focusedPanel; }

		Panel(const std::string& a_name);
		virtual ~Panel();

		void setVisible(const bool a_visible) { m_visible = a_visible; }

		Galactose::Vector2 padding() const;
		void setPadding(const Galactose::Vector2& a_padding) { m_padding = a_padding; }

		void update();
		virtual void onUpdate() {}
		virtual void onFocusOut() {}

	private:
		static Panel* s_focusedPanel;

		bool m_visible = true;
		std::string m_name;
		Galactose::Vector2 m_padding;
	};
}
