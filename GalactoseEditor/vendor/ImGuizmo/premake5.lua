project "ImGuizmo"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "Off"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"ImGuizmo/ImGuizmo.h",
		"ImGuizmo/ImGuizmo.cpp"
	}
	
	includedirs
	{
		"../ImGui/ImGui"
	}

	links
	{
		"ImGui"
	}
	
	filter "system:windows"
		systemversion "latest"

	filter "system:linux"
		pic "On"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"
		