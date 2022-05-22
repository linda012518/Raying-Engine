project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/Raying/vendor/spdlog/include",
		"%{wks.location}/Raying/src",
		"%{wks.location}/Raying/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
	}

	links
	{
		"Raying"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "Raying_Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "Raying_Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "Raying_Dist"
		runtime "Release"
		optimize "on"
