project "yaml-cpp"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "Off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	defines {
		"YAML_CPP_STATIC_DEFINE"
	}
	
	files
	{
		"yaml-cpp/include/**.h",
		"yaml-cpp/src/**.h",
		"yaml-cpp/src/**.cpp"
	}
	
	includedirs
	{
		"yaml-cpp/include"
	}
	
	filter "system:linux"
		pic "On"
		systemversion "latest"

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
