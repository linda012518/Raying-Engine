include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "Raying Engine"
	architecture "x64"
	startproject "Editor"
	
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "vendor/premake"
	include "Raying/vendor/GLFW" --Include GLFW premake5.lua
	include "Raying/vendor/GLAD" 
	include "Raying/vendor/imgui"
	include "Raying/vendor/yaml-cpp"

group ""

include "Raying"
include "Sandbox"
include "Editor"
