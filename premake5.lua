workspace "peer"
    architecture "x64"

    configurations {
        "Debug",
        "Release"
    }

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "peer"
    location "peer"
    kind "StaticLib"
    language "C"

	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-intermediate/" .. outputDir .. "/%{prj.name}")

	debugdir ("bin/" .. outputDir .. "/%{prj.name}")

    files
	{
		"%{prj.name}/include/**.h",
		"%{prj.name}/src/**.c",
		"%{prj.name}/src/vendor/**.c",
	}

    includedirs
	{
		"peer/include",
		"peer/include/vendor"
	}

    filter "configurations:Debug"
		defines
		{
			"PEER_DEBUG",
		}
		symbols "On"

	-- If the project is generated with the Debug configuration it will apply these rules
	filter "configurations:Release"
		optimize "On"

project "peer-project"
        location "peer-project"
        kind "ConsoleApp"
        language "C"
    
        targetdir ("bin/" .. outputDir .. "/%{prj.name}")
        objdir ("bin-intermediate/" .. outputDir .. "/%{prj.name}")
    
        debugdir ("bin/" .. outputDir .. "/%{prj.name}")
    
        files
        {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.c",
        }
        
        includedirs
        {
            "peer-project/include",
            "peer-project/include/vendor",
            "peer/include/vendor",
            "peer/include"
        }
        
        links ("peer")
    
        filter "system:linux"
            postbuildcommands ("cp -R assets/* ../bin/" .. outputDir .. "/%{prj.name}/")
    
        filter "configurations:Debug"
            defines
            {
                "PEER_DEBUG",
            }
            symbols "On"
    
        filter "configurations:Release"
            optimize "On"
