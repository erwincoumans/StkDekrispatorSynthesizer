	
		project "App_SimpleDemoSelector"

		language "C++"
				
		kind "ConsoleApp"

  	includedirs {
                ".",
                "../../btgui",
		"../../external/mujoco/source",
		"../../external/mujoco/sdk/ccd",
				"../../external/inc",
                }

		links{"Bullet3AppSupport","gwen", "OpenGL_Window","Bullet3Common"}
		initOpenGL()
		initGlew()

    
		files {
		"*.cpp",
		"*.cc",
		"*.h",
	
		
		}

if os.is("Linux") then 
	initX11()
end
if os.is("MacOSX") then
	links{"Cocoa.framework"}
end
