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

include "Galactose/premake5.lua"
include "GalactoseEditor/premake5.lua"
