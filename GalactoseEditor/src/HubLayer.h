#pragma once

#include "ImGuiLayer.h"

#include <filesystem>
#include <map>

namespace GalactoseEditor {
	class HubLayer : public ImGuiLayer {
	public:
		HubLayer(Galactose::Window* window);
		~HubLayer();

		std::filesystem::path projectFilePath() { return m_projectPath; }

		void updateContent() override;

	private:
		struct ProjectInfo {
			std::filesystem::path path;
			std::string time;
		};

		static std::string timeToString(const int64_t ms);

		void insertProject();

		inline static const std::string RECENT_PROJECTS_FILENAME = "RecentProjects.txt";

		std::filesystem::path m_projectPath;
		Galactose::Window* m_window = nullptr;
		// key: time since epoch, value: project path
		std::map<int64_t, ProjectInfo, std::greater<int64_t>> m_projects; // descending order
	};
}
