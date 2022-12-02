#pragma once

#include <string>

namespace GalactoseEditor {
	class Widget {
	public:
		Widget(const std::string& a_name) : m_name(a_name) {}

		void setVisible(const bool a_visible) { m_visible = a_visible; }
		void update();
		virtual void onUpdate() {}

	private:
		bool m_visible = true;
		std::string m_name;
	};
}
