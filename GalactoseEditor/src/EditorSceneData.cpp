#include "EditorSceneData.h"

#include <Core/Global.h>

#include <nfd.hpp>

namespace GalactoseEditor {
	void EditorSceneData::saveAndPrint() {
		m_scene->save(m_filePath);
		std::cout << "Scene saved to '" << m_filePath << "'." << std::endl;
	}

	void EditorSceneData::saveAs() {
		nfdchar_t* path;
		nfdfilteritem_t filter = { "Scene", "yaml" };
		const auto result = NFD_SaveDialog(&path, &filter, 1, nullptr, m_scene->name().c_str());
		GT_ASSERT(result != NFD_ERROR, NFD_GetError());

		if (result == NFD_OKAY) {
			m_filePath = path;
			saveAndPrint();
		}
	}

	void EditorSceneData::save() {
		if (m_filePath.empty())
			saveAs();
		else
			saveAndPrint();
	}
}
