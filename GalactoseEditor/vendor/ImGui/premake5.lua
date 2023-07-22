project "ImGui"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "Off"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"ImGui/**.h",		
		"ImGui/**.cpp"
	}
	
	excludes
	{
		"ImGui/backends/**.h",
		"ImGui/backends/**.cpp",
		"ImGui/examples/**.h",
		"ImGui/examples/**.cpp",
		"ImGui/misc/**.h",
		"ImGui/misc/**.cpp"
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
		