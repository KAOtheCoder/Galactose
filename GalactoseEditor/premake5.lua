project "GalactoseEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "Off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/Galactose/src",
		"%{wks.location}/Galactose/vendor/glm",
		"%{wks.location}/Galactose/vendor/entt/src/entt"
	}

	links
	{
		"Galactose"
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
