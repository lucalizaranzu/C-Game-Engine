workspace "Shmingo"
   architecture "x64"

   configurations {
      "Debug",
      "Release",
      "Distribution"
   }

outputDirectory = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Shmingo"
   location "Shmungus"
   kind "ConsoleApp"
   language "C++"

   targetdir ("bin/" .. outputDirectory .."/%{prj.name}")
   objdir ("bin-obj/" .. outputDirectory .."/%{prj.name}")

   pchheader ("sepch.h")
   pchsource("Shmungus/src/engine/core/sepch.cpp")

   files {
      "Shmungus/src/**.cpp",
      "Shmungus/src/**.c",
      "Shmungus/src/**.h",
      "Shmungus/shaders/**.glsl",
   }

   includedirs {
      -- Main include directories
      "Shmungus/src/**",
      "Libraries/include",

      -- Directories of libraries which need to be included in root
      "Libraries/include/freetype",
      "Libraries/include/stb_image",
      "Libraries/include/glad"
   }

   libdirs {
      "Libraries/lib"
   }

   links {
      "glfw3",
      "freetype",
      "opengl32"
   }

   filter "files:**.c"
      flags { "NoPCH" }

   filter "system:windows"
      cppdialect "C++20"
      systemversion "latest"

   filter "configurations:Debug"
      runtime "Debug"
      symbols "On"
      defines { "se_DEBUG" }

   filter "configurations:Release"
      runtime "Release"
      optimize "On"
      defines { "se_RELEASE" }

   filter "configurations:Distribution"
      runtime "Release"
      optimize "Full"
      symbols "Off"
      defines { "se_DISTRIBUTION" }
