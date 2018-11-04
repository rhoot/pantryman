project "pantryman_test"
    kind "ConsoleApp"

    includedirs {
        "../include",
    }

    files {
        "../test/**.cpp",
    }

    links {
        "pantryman",
    }

    configuration {"macosx"}
        linkoptions {
            "-framework AppKit",
            "-framework IOKit",
        }

    configuration{}
