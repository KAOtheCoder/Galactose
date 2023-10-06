#pragma once

#include "SideBarItem.h"

namespace GalactoseEditor {
	class EditorContext;

	class StatusBar : public SideBarItem {
	public:
		StatusBar(const std::shared_ptr<EditorContext>& editorContext);

		float size() const override;

		void update() override;

	private:
		std::shared_ptr<EditorContext> m_editorContext;
	};
}
