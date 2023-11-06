#include "FileDialog.h"

#include <nfd.hpp>

#include <iostream>
#include <vector>

namespace GalactoseEditor {
	namespace FileDialog {
		bool init() {
			const bool result = NFD_Init() == NFD_OKAY;

			if (!result)
				std::cerr << "Failed to initialze NFD." << std::endl;

			return result;
		}

		void uninit() {
			NFD_Quit();
		}

		std::vector<nfdfilteritem_t> convertFilters(const std::vector<Filter>& a_filters) {
			std::vector<nfdfilteritem_t> filterItems;
			filterItems.reserve(a_filters.size());
			for (const auto& filter : a_filters)
				filterItems.emplace_back(filter.name.c_str(), filter.spec.c_str());

			return filterItems;
		}

		const nfdu8char_t* convertString(const std::string& a_string) { return a_string.empty() ? nullptr : a_string.c_str(); }

		std::string convertPath(nfdchar_t* a_path, const nfdresult_t result) {
			if (result == NFD_OKAY) {
				const std::string stringPath(a_path);
				NFD_FreePath(a_path);
				return stringPath;
			}
			else if (result == NFD_ERROR) {
				std::cerr << NFD_GetError() << std::endl;
			}

			return std::string();
		}

		std::string open(const std::vector<Filter>& a_filters, const std::string& a_defaultPath) {
			nfdchar_t* path;
			const auto& filterItems = convertFilters(a_filters);
			const auto result = NFD_OpenDialog(&path, filterItems.data(), (nfdfiltersize_t)(filterItems.size()), convertString(a_defaultPath));

			return convertPath(path, result);
		}

		std::vector<std::string> openMultiple(const std::vector<Filter>& a_filters, const std::string& a_defaultPath) {
			const nfdpathset_t* paths;
			const auto& filterItems = convertFilters(a_filters);
			const auto result = NFD_OpenDialogMultiple(&paths, filterItems.data(), (nfdfiltersize_t)(filterItems.size()), convertString(a_defaultPath));
			std::vector<std::string> stringPaths;

			if (result == NFD_OKAY) {
				nfdpathsetsize_t pathCount;
				NFD_PathSet_GetCount(paths, &pathCount);
				stringPaths.reserve(pathCount);

				for (nfdpathsetsize_t i = 0; i < pathCount; ++i) {
					nfdchar_t* path;
					NFD_PathSet_GetPath(paths, i, &path);
					stringPaths.push_back(path);
					NFD_PathSet_FreePath(path);
				}

				NFD_PathSet_Free(paths);
			}
			else if (result == NFD_ERROR) {
				std::cerr << NFD_GetError() << std::endl;
			}

			return stringPaths;
		}

		std::string save(const std::vector<Filter>& a_filters, const std::string& a_defaultPath, const std::string& a_defaultName) {
			nfdchar_t* path;
			const auto& filterItems = convertFilters(a_filters);
			const auto result = NFD_SaveDialog(&path, filterItems.data(), (nfdfiltersize_t)(filterItems.size()), convertString(a_defaultPath), convertString(a_defaultName));

			return convertPath(path, result);
		}
	}
}