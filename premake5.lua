-- https://github.com/premake/premake-core/wiki

-- require "./premake5/premake-androidmk/androidmk"
-- http://gulu-dev.com/post/2016-04-17-premake-android-mk

local action = _ACTION or ""

solution "Eyeo2012"
    configurations { "Debug", "Release" }
    language "C++"

    location (action)
    platforms {"x64"}

    configuration "Debug"
        optimize "Debug"
        defines { "DEBUG" }
        symbols "On"
        targetsuffix "-d"

    configuration "Release"
        defines { "NDEBUG" }
        optimize "On"

    configuration "vs*"
        defines {
            "_CRT_SECURE_NO_WARNINGS",
            "WIN32",
            "VK_USE_PLATFORM_WIN32_KHR",
            "NOMINMAX",
            "_USE_MATH_DEFINES",
        }

        configuration "x64"
            targetdir ("x64")

    flags {
        "MultiProcessorCompile",
        "StaticRuntime"
    }

    includedirs {
        "../Cinder/include",
    }

    function create_example_project( example_path )
        -- leaf_name = string.sub(example_path, string.len("examples/") + 1)
        leaf_name = example_path

        project (leaf_name)
            kind "WindowedApp"
            files { 
                leaf_name .. "/include/**",
                leaf_name .. "/src/**",
            }

            includedirs {
                leaf_name .. "/include",
            }

            configuration "Debug"
                libdirs {
                    "../Cinder/lib/msw/x64/Debug/v140"
                }
                links {
                    "cinder",
                }

            configuration "Release"
                libdirs {
                    "../Cinder/lib/msw/x64/Release/v140"
                }
                links {
                    "cinder",
                }

    end

    local examples = os.matchdirs("*")
    for _, example in ipairs(examples) do
        if example ~= ".git" and example ~= "vs2015" and example ~= "x64" then
            create_example_project(example)
        end
    end