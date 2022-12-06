#pragma once

#include <Math/Math.h>

#include <string>

namespace GalactoseEditor {
	class Widget {
	public:
		Widget(const std::string& a_name);

		void setVisible(const bool a_visible) { m_visible = a_visible; }

		Galactose::Vector2 padding() const;
		void setPadding(const Galactose::Vector2& a_padding) { m_padding = a_padding; }

		void update();
		virtual void onUpdate() {}

	private:
		bool m_visible = true;
		std::string m_name;
		Galactose::Vector2 m_padding;
	};
}
