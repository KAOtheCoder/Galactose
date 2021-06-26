project "Galactose"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "Off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "GalactosePCH.h"
	pchsource "GalactosePCH.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	defines
	{
		"GLFW_INCLUDE_NONE"
	}

	links
	{
		"GLFW",
		"opengl32.lib"
	}

	includedirs
	{
		"src",
		"vendor/GLFW/include"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	group "Dependencies"
		include "Galactose/vendor/GLFW"
	group ""