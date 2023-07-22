--
-- Remember my location; I will need it to locate sub-scripts later.
--

local scriptPath = _SCRIPT_DIR

--
-- Register supporting actions and options.
--

newaction {
	trigger = "embed",
	description = "Embed scripts in scripts.c; required before release builds",
	execute = function ()
		include (path.join(scriptPath, "Premake5/scripts/embed.lua"))
	end
}

newaction {
	trigger = "package",
	description = "Creates source and binary packages",
	execute = function ()
		include (path.join(scriptPath, "Premake5/scripts/package.lua"))
	end
}

project "Premake5"
	language    "C"
	kind        "StaticLib"
	staticruntime "Off"
	includedirs { "Premake5/contrib/lua/src", "Premake5/contrib/luashim" }
	links       { "lua-lib" }

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"*.txt", 
		"**.lua",
		"Premake5/src/**.h", 
		"Premake5/src/**.c",
		"Premake5/modules/**"
	}

	excludes
	{
		"Premake5/contrib/**.*",
		"Premake5/binmodules/**.*",
		"Premake5/src/host/premake_main.c"
	}

	defines 
	{
		"_CRT_SECURE_NO_WARNINGS",
		"_CRT_NONSTDC_NO_WARNINGS"
	}
	
	filter "configurations:Debug"
		debugargs   { "--scripts=%{prj.location}/%{path.getrelative(prj.location, prj.basedir)}", "test" }
		debugdir    "."
		defines     "_DEBUG"
		symbols "On"
		
	filter "configurations:Release"
		defines     "NDEBUG"
		optimize    "Full"
		flags       { "NoBufferSecurityCheck", "NoRuntimeChecks" }
		
	filter "system:windows"
		links       { "ole32", "ws2_32", "advapi32", "version" }
		files { "Premake5/src/**.rc" }

	filter { "system:windows", "configurations:Release" }
		flags       { "NoIncrementalLink" }
		
	filter { "system:windows", "configurations:Release", "toolset:not mingw" }
		flags		{ "LinkTimeOptimization" }
		
	filter "toolset:mingw"
		links		{ "crypt32" }

	filter "system:linux or bsd or hurd"
		defines     { "LUA_USE_POSIX", "LUA_USE_DLOPEN" }
		links       { "m" }
		linkoptions { "-rdynamic" }

	filter "system:linux or hurd"
		links       { "dl", "rt" }

	filter "system:macosx"
		defines     { "LUA_USE_MACOSX" }
		links       { "CoreServices.framework", "Foundation.framework", "Security.framework", "readline" }

	filter "system:linux"
		links		{ "uuid" }

	filter { "system:macosx", "action:gmake" }
		toolset "clang"

	filter { "system:solaris" }
		links       { "m", "socket", "nsl" }

	filter "system:aix"
		defines     { "LUA_USE_POSIX", "LUA_USE_DLOPEN" }
		links       { "m" }

	filter "system:haiku"
		defines     { "LUA_USE_POSIX", "LUA_USE_DLOPEN", "_BSD_SOURCE" }
		links       { "network", "bsd" }
		
	group "Dependencies"
		include "GalactoseEditor/vendor/Premake5/Premake5/contrib/lua"
		include "GalactoseEditor/vendor/Premake5/Premake5/contrib/luashim"
	group ""
