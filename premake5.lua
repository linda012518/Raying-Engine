include "./vendor/premake/premake_customization/solution_items.lua"

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

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Raying/vendor/GLFW/include"
IncludeDir["GLAD"] = "%{wks.location}/Raying/vendor/GLAD/include"
IncludeDir["ImGui"] = "%{wks.location}/Raying/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/Raying/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/Raying/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/Raying/vendor/entt/include"

group "Dependencies"
	include "vendor/premake"
	include "Raying/vendor/GLFW" --Include GLFW premake5.lua
	include "Raying/vendor/GLAD" 
	include "Raying/vendor/imgui"

group ""

include "Raying"
include "Sandbox"
include "Editor"
