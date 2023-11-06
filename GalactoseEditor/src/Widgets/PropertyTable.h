#pragma once

#include <filesystem>
#include <unordered_map>

namespace Galactose {
	class Texture;
}

namespace GalactoseEditor {
	namespace FileDialog {
		struct Filter;
	}

	class PropertyTable {
	public:
		PropertyTable() = delete;

		static void init();

		static bool beginTable(const char* name);
		static void endTable();

		static void label(const char* label);
		static bool iconButton(const char* icon);
		static bool fileInput(const char* label, std::filesystem::path& path, const std::vector<FileDialog::Filter>& filters, const std::string& emptyText = "");

	private:
		inline static std::unordered_map<std::string, std::shared_ptr<Galactose::Texture>> s_icons;
	};
}
