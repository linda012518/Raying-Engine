workspace "Raying Engine"
	architecture "x64"
	startproject "Sandbox"
	
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Raying/vendor/GLFW/include"
IncludeDir["GLAD"] = "Raying/vendor/GLAD/include"
IncludeDir["ImGui"] = "Raying/vendor/imgui"
IncludeDir["glm"] = "Raying/vendor/glm"

include "Raying/vendor/GLFW" --Include GLFW premake5.lua
include "Raying/vendor/GLAD" 
include "Raying/vendor/imgui"

project "Raying"
	location "Raying"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "hzpch.h"
	pchsource "Raying/src/hzpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/**.hpp",
		"%{prj.name}/vendor/glm/**.inl"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links 
	{ 
		"GLFW",
		"GLAD",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"Raying_Platform_Windows",
			"Raying_Build_Dll",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
		}

	filter "configurations:Debug"
		defines "Raying_Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "Raying_Release"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "Raying_Dist"
		runtime "Release"
		optimize "On"
	
	--filter {"system:windows", "configurations:Release"}
		--buildoptions "/MT" 多线程调试MD/MT

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Raying/vendor/spdlog/include",
		"Raying/src",
		"Raying/vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Raying"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"Raying_Platform_Windows"
		}

	filter "configurations:Debug"
		defines "Raying_Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "Raying_Release"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "Raying_Dist"
		runtime "Release"
		optimize "On"
