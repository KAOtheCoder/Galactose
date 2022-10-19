project "Galactose"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "GalactosePCH.h"
	pchsource "src/GalactosePCH.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
		"vendor/stb/**.h",
		"vendor/stb/**.cpp"
	}

	defines
	{
		"GLFW_INCLUDE_NONE"
	}

	links
	{
		"GLFW",
		"Glad",
		"opengl32.lib"
	}

	includedirs
	{
		"src",
		"vendor/GLFW/include",
		"vendor/glad/include",
		"vendor/glm",
		"vendor/stb",
		"vendor/entt/src/entt"
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
		symbols "on"

	filter "configurations:Release"
		defines "GT_RELEASE"
		runtime "Release"
		optimize "on"

	group "Dependencies"
		include "Galactose/vendor/GLFW"
		include "Galactose/vendor/glad"
	group ""