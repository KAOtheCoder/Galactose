#pragma once

#include <filesystem>
#include <unordered_map>

#include <Galactose/Math/Math.h>

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

		static void openPopup(const char* label);

		static void label(const char* label);
		static bool iconButton(const char* icon);
		static bool fileInput(const char* label, std::filesystem::path& path, const std::vector<FileDialog::Filter>& filters, const std::string& emptyText = "");
		static bool sliderFloat(const char* label, float& value, const float min, const float max);
		static bool dragFloat(const char* label, float& value, const float speed = 0.1f, const float min = 0, const float max = 0);
		static bool dragVector(const char* label, const int axisCount, float* values, const bool colored = false);
		static bool colorButton(const char* descId, Galactose::Vector4& color);
		static bool checkBox(const char* label, bool& value);

	private:
		static bool spanDragFloat(const char* label, float& value, const float speed = 0.1f, const float min = 0, const float max = 0);
		static bool dragVectorAxis(const int axis, float& value, const Galactose::Vector4& color = { 0.f, 0.f, 0.f, 0.f }, const Galactose::Vector4& hoverColor = { 1.f, 1.f, 1.f, 0.1f });

		inline static std::unordered_map<std::string, std::shared_ptr<Galactose::Texture>> s_icons;
	};
}
