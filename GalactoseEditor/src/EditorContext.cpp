#include "EditorContext.h"

#include <Core/Global.h>

#include <nfd.hpp>

using namespace Galactose;

namespace GalactoseEditor {
	void EditorContext::saveAndPrint() {
		m_scene->save(m_filePath);
		std::cout << "Scene saved to '" << m_filePath << "'." << std::endl;
	}

	void EditorContext::newScene() {
		m_scene = std::make_shared<Scene>("Untitled");
		m_filePath.clear();
		m_selectedEntity = nullptr;
	}

	void EditorContext::openScene() {
		nfdchar_t* path;
		nfdfilteritem_t filter = { "Scene", "yaml" };
		const auto result = NFD_OpenDialog(&path, &filter, 1, nullptr);
		GT_ASSERT(result != NFD_ERROR, NFD_GetError());

		auto scene = std::make_shared<Scene>();
		if (result == NFD_OKAY && scene->load(path)) {
			m_scene = scene;
			m_filePath = path;
			m_selectedEntity = nullptr;
			std::cout << "Scene '" << m_filePath << "' loaded." << std::endl;
		}
	}

	void EditorContext::saveAs() {
		nfdchar_t* path;
		nfdfilteritem_t filter = { "Scene", "yaml" };
		const auto result = NFD_SaveDialog(&path, &filter, 1, nullptr, m_scene->name().c_str());
		GT_ASSERT(result != NFD_ERROR, NFD_GetError());

		if (result == NFD_OKAY) {
			m_filePath = path;
			saveAndPrint();
		}
	}

	void EditorContext::save() {
		if (m_filePath.empty())
			saveAs();
		else
			saveAndPrint();
	}

	void EditorContext::setRunning(const bool a_running) {
		if (a_running == m_running)
			return;

		if (!a_running)
			m_scene->time().suspend();

		m_running = a_running;
	}
}
