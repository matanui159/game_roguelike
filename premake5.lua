require "font.convert"

workspace "roguelike"
	configurations { "Debug", "Release" }

project "roguelike"
	kind "WindowedApp"
	language "C"
	cdialect "C99"
	files "src/**.c"
	objdir "bin/%{cfg.buildcfg}/obj"
	targetdir "bin/%{cfg.buildcfg}"

	linkoptions {
		"-nostdlib",
		"-Wl,-e_display__entry"
	}

	links {
		"kernel32",
		"user32",
		"gdi32"
	}

	filter "Debug"
		symbols "On"

	filter "Release"
		optimize "Size"
		flags "LinkTimeOptimization"
		postbuildcommands {
			"upx --ultra-brute bin/%{cfg.buildcfg}/%{prj.name}.exe"
		}