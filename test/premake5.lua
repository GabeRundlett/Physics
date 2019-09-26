project "test"
	kind "ConsoleApp"
	language "C++"
	targetdir("%{wks.location}/build/bin/" .. outputdir .. "/%{prj.name}")
	objdir("%{wks.location}/build/bin/intermediates/" .. outputdir .. "/%{prj.name}")
    flags { "MultiProcessorCompile" }
    files { "src/**.hpp", "src/**.cpp" }
    includedirs { "%{inc.debug}", "%{inc.phsyics}" }
    links { "debug", "physics" }
    warnings "Extra"
	filter "configurations:Debug"
        defines "_CONFIG_DEBUG"
        optimize "Off"
        symbols "On"
	filter "configurations:DebugOptimized"
        flags { "LinkTimeOptimization" }
        defines "_CONFIG_DEBUG_OPTIMIZED"
		optimize "Off"
		symbols "Off"
    filter "configurations:Release"
        flags { "LinkTimeOptimization" }
		defines "_CONFIG_RELEASE"
		optimize "Full"
		symbols "Off"
	filter "system:windows"
		systemversion "latest"
		defines "_CONFIG_PLATFORM_WINDOWS"
	filter "system:linux"
		defines "_CONFIG_PLATFORM_LINUX"
	filter "system:macosx"
		defines "_CONFIG_PLATFORM_MACOS"
