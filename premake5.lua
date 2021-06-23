workspace "Galactose"
	architecture "x86_64"
	startproject "GalactoseEditor"

	configurations
	{
		"Debug",
		"Release"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "GalactoseEditor/premake5.lua"
