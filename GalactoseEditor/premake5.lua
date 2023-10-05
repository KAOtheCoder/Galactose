project "GalactoseEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "Off"
	ignoredefaultlibraries { "MSVCRT" }
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
		"vendor/ImGui/ImGui/backends/imgui_impl_glfw.h",
		"vendor/ImGui/ImGui/backends/imgui_impl_glfw.cpp",
		"vendor/ImGui/ImGui/backends/imgui_impl_opengl3.h",
		"vendor/ImGui/ImGui/backends/imgui_impl_opengl3.cpp"
	}

	includedirs
	{
		"%{wks.location}/Galactose/src",
		"%{wks.location}/Galactose/vendor/glm",
		"%{wks.location}/Galactose/vendor/entt/src/entt",
		"%{wks.location}/Galactose/vendor/GLFW/GLFW/include",
		"src",
		"vendor/ImGui/ImGui",
		"vendor/ImGuizmo/ImGuizmo",
		"vendor/NativeFileDialogExtended/NativeFileDialogExtended/src/include",
		"vendor/Premake5/Premake5/src",
		"vendor/Premake5/Premake5/contrib/lua/src"
	}

	links
	{
		"Galactose",
		"GLFW",
		"ImGui",
		"ImGuizmo",
		"NativeFileDialogExtended",
		"Premake5"
	}
	
	defines {
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_DLL"
	}
	
	filter "system:windows"
		defines "GT_WINDOWS"
		systemversion "latest"

	filter "system:linux"
		defines "GT_LINUX"
		systemversion "latest"

	filter "configurations:Debug"
		defines "GT_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "GT_RELEASE"
		runtime "Release"
		optimize "On"

	group "Dependencies"
		include "GalactoseEditor/vendor/ImGui"
		include "GalactoseEditor/vendor/ImGuizmo"
		include "GalactoseEditor/vendor/NativeFileDialogExtended"
		include "GalactoseEditor/vendor/Premake5"
	group ""
	