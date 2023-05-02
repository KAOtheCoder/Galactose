project "Galactose"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "Off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
		"src/**.glsl",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
		"vendor/stb/**.h",
		"vendor/stb/**.cpp"
	}

	defines
	{
		"GLFW_INCLUDE_NONE",
		"YAML_CPP_STATIC_DEFINE"
	}

	links
	{
		"GLFW",
		"Glad",
		"opengl32.lib",
		"yaml-cpp"
	}

	includedirs
	{
		"src",
		"vendor/GLFW/GLFW/include",
		"vendor/glad/include",
		"vendor/glm",
		"vendor/stb",
		"vendor/entt/src/entt",
		"vendor/yaml-cpp/yaml-cpp/include"
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
		include "Galactose/vendor/yaml-cpp"
	group ""
	