project "GalactoseEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "Off"

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
		"vendor/NativeFileDialogExtended/NativeFileDialogExtended/src/include"
	}

	links
	{
		"Galactose",
		"GLFW",
		"ImGui",
		"ImGuizmo",
		"NativeFileDialogExtended"
	}
	
	defines {
		"GLFW_DLL"
	}
	
	filter "system:windows"
		defines "GT_WINDOWS"
		systemversion "latest"
		postbuildcommands { 
			("{echo} postbuild"),
			("{copy} %{wks.location}/Galactose/bin/" .. outputdir .. "/Galactose/*.dll %{cfg.targetdir}"),
			("{copy} %{wks.location}/Galactose/vendor/GLFW/bin/" .. outputdir .. "/GLFW/*.dll %{cfg.targetdir}")
		}
		cleanextensions { ".dll", ".pdb" }

	filter "system:linux"
		defines "GT_LINUX"
		systemversion "latest"

	filter "configurations:Debug"
		defines "GT_DEBUG"
		runtime "Debug"
		symbols "On"
		postbuildcommands { 
			("{copy} %{wks.location}/Galactose/bin/" .. outputdir .. "/Galactose/*.pdb %{cfg.targetdir}"),
			("{copy} %{wks.location}/Galactose/vendor/GLFW/bin/" .. outputdir .. "/GLFW/*.pdb %{cfg.targetdir}")
		}

	filter "configurations:Release"
		defines "GT_RELEASE"
		runtime "Release"
		optimize "On"

	group "Dependencies"
		include "GalactoseEditor/vendor/ImGui"
		include "GalactoseEditor/vendor/ImGuizmo"
		include "GalactoseEditor/vendor/NativeFileDialogExtended"
	group ""
	