project "Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

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
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGuizmo}"
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

		postbuildcommands
		{
			"{COPYDIR} \"%{LibraryDir.VulkanSDK_DebugDLL}\" \"%{cfg.targetdir}\""
		}

	filter "configurations:Release"
		defines "Raying_Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "Raying_Dist"
		runtime "Release"
		optimize "on"
