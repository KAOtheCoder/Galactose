project "NativeFileDialogExtended"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "Off"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"NativeFileDialogExtended/src/include/nfd.h",
		"NativeFileDialogExtended/src/include/nfd.hpp"
	}
	
	includedirs
	{
		"NativeFileDialogExtended/src/include"
	}
	
	filter "system:windows"
		systemversion "latest"
		
		files
		{
			"NativeFileDialogExtended/src/nfd_win.cpp"
		}

	filter "system:linux"
		pic "On"
		systemversion "latest"
		
		files
		{
			"NativeFileDialogExtended/src/nfd_gtk.cpp"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"
		