workspace "Shmingo"
	architecture "x64"

	configurations{

		"Debug",
		"Release",
		"Distribution"
	}

outputDirectory = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
srcDirectory = "Shmungus/src/"
libDirectory = "Libraries/"

project "Shmingo"
	location "Shmungus"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputDirectory .."/%{prj.name}")
	objdir ("bin-obj/" .. outputDirectory .."/%{prj.name}")

	pchheader ("sepch.h")
	pchsource(srcDirectory .. "engine/sepch.cpp")

	files{

		(srcDirectory .. "**/*.h"),
		(srcDirectory .. "**/*.cpp"),
		(srcDirectory .. "*.cpp"),
		"Shmungus/shaders/**.glsl",
		libDirectory .. "lib/vendors/*.cpp",
		libDirectory .. "lib/vendors/*.c",


	}

	includedirs{

		libDirectory .. "include",
		(srcDirectory .. "**"),
		"Shmungus/shaders",
		libDirectory .. "vendors/stb_image"
	}

	libdirs{

		libDirectory .. "lib"
	}

	links{

		libDirectory .. "lib/configs/glfw3_mt.lib",
		libDirectory .. "lib/configs/glfw3.dll"
	}

	filter "files:**.c"
		flags {"NoPCH"}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"
		filter "system:windows"

	filter "configurations:Debug"
		defines{
			"se_DEBUG"
		}
		
	filter "configurations:Release"
		optimize "On"
		defines{
			"se_RELEASE"
		}
		

	filter "configurations:Distribution"
		optimize "On"
		symbols "Off"
		defines{
			"se_DISTRIBUTION"
		}