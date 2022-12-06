#pragma once

#include <Math/Math.h>

#include <string>

namespace GalactoseEditor {
	class Widget {
	public:
		static Widget* focusedWidget() { return s_focusedWidget; }

		Widget(const std::string& a_name);
		virtual ~Widget();

		void setVisible(const bool a_visible) { m_visible = a_visible; }

		Galactose::Vector2 padding() const;
		void setPadding(const Galactose::Vector2& a_padding) { m_padding = a_padding; }

		void update();
		virtual void onUpdate() {}
		virtual void onFocusOut() {}

	private:
		static Widget* s_focusedWidget;

		bool m_visible = true;
		std::string m_name;
		Galactose::Vector2 m_padding;
	};
}
