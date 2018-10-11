--
-- Copyright (c) 2018 Johan Sköld
-- License: https://opensource.org/licenses/ISC
--

solution "pantryman"
    location  "../.build/prj"
    objdir    "../.build/obj"
    targetdir "../.build/out"

    language "C++"
    configurations { "Debug", "Release" }
    windowstargetplatformversion "10.0.17134.0"

    if os.get() == "windows" then
        platforms {"x64"}
    else
        platforms {"Native"}
    end

    flags {
        "Cpp11",
        "ExtraWarnings",
        "FatalWarnings",
        "NoExceptions",
        "NoPCH",
        "NoRTTI",
        "StaticRuntime",
        "Symbols",
    }

    includedirs {
        "../include",
    }

    configuration {"Release"}
        flags {"OptimizeSpeed"}

    configuration {"vs*"}
        buildoptions {
            -- Triggered by Windows headers:
            -- warning C4530: C++ exception handler used, but unwind semantics are not enabled.
            "/wd4530",
        }
        defines {
            "_SCL_SECURE_NO_WARNINGS",
            "_CRT_SECURE_NO_WARNINGS",
        }

    project "pantryman"
        kind "StaticLib"

        files {
            "../include/**.hpp",
            "../src/**.hpp",
            "../src/**.cpp",
        }

    project "pantryman_test"
        kind "ConsoleApp"

        files {
            "../test/**.cpp",
        }

        links {
            "pantryman",
        }
