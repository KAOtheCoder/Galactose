#pragma once

#include "Panel.h"

#include <filesystem>

namespace GalactoseEditor {
	class Preferences : public Panel {
	public:
		Preferences();

		void onUpdate() override;
		void save();

		std::filesystem::path visualStudioPath() const { return m_visualStudioPath; }
		void setVisualStudioPath(const std::filesystem::path& path);

	private:
		inline static const std::string PREFERENCES_FILENAME = "Preferences.yaml";

		bool m_changed = false;
		std::filesystem::path m_visualStudioPath;
	};
}
