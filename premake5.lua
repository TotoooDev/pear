workspace "pear"
    configurations { "debug", "release" }
    platforms { "opengl" }

bin_directory = "../bin/%{cfg.buildcfg}-%{cfg.system}/%{prj.name}"
bin_intermediate_directory = "../obj/%{cfg.buildcfg}-%{cfg.system}/%{prj.name}"

include "pear/vendor/glfw"
include "pear/vendor/lua"

include "pear"
include "editor"
