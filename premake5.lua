workspace "BooyerMoore"
    configurations { "Debug" }

project "Horspool"
    kind "ConsoleApp"
    language "C"
    targetdir "bin/%{cfg.buildcfg}"

    files { "src/**.c" }
    includedirs { "vendor/" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
