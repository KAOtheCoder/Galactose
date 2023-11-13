#pragma once

#include "Panel.h"

#include <filesystem>

// TODO: Separate Preferences and PreferencesPanel
namespace GalactoseEditor {
	class Preferences : public Panel {
	public:
		static Preferences* instance() { return s_instance; }

		Preferences();

		void onUpdate() override;
		void save();

		std::filesystem::path visualStudioPath() const { return m_visualStudioPath; }
		void setVisualStudioPath(const std::filesystem::path& path);

	private:
		inline static const std::string PREFERENCES_FILENAME = "Preferences.yaml";
		inline static Preferences* s_instance = nullptr;

		bool m_changed = false;
		std::filesystem::path m_visualStudioPath;
	};
}
