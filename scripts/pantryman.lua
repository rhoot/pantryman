project "pantryman"
    kind "StaticLib"

    includedirs {
        "../include",
    }

    files {
        "../include/**.hpp",
        "../src/**.hpp",
        "../src/**.cpp",
        "../src/**.mm",
    }
