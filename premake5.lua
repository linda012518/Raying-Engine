workspace "Raying Engine"
	architecture "x64"
	
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Raying"
	location "Raying"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "hzpch.h"
	pchsource "Raying/src/hzpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "10.0.17763.0"

		defines
		{
			"Raying_Platform_Windows",
			"Raying_Build_Dll"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "Raying_Debug"
		symbols "On"
	filter "configurations:Release"
		defines "Raying_Release"
		optimize "On"
	filter "configurations:Dist"
		defines "Raying_Dist"
		optimize "On"
	
	--filter {"system:windows", "configurations:Release"}
		--buildoptions "/MT" 多线程调试MD/MT

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"

	language "C++"

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
		"Raying/src"
	}

	links
	{
		"Raying"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "10.0.17763.0"

		defines
		{
			"Raying_Platform_Windows"
		}

	filter "configurations:Debug"
		defines "Raying_Debug"
		symbols "On"
	filter "configurations:Release"
		defines "Raying_Release"
		optimize "On"
	filter "configurations:Dist"
		defines "Raying_Dist"
		optimize "On"
