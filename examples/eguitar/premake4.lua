	
		project "App_ElectricGuitar"

		language "C++"
				
		kind "ConsoleApp"

  	includedirs {
                ".",
                "../../stk/include"
                }
			
		links{ "stk"}
		defines {"__STK_REALTIME__"}	
		files {
		"**.cpp",
		"**.h",
		}
		
		if os.is("Windows") then
			links {"winmm","Wsock32","dsound"}
			defines {"WIN32","__WINDOWS_MM__","__LITTLE_ENDIAN__","__WINDOWS_DS__"}
		end
		
if os.is("Linux") then initX11() 
    defines  {"__OS_LINUX__","__LINUX_JACK__","__LITTLE_ENDIAN__"}
	links {"pthreads","jack"}
end

if os.is("MacOSX") then
	links{"Cocoa.framework"}
	links{"CoreAudio.framework", "coreMIDI.framework", "Cocoa.framework"}

	defines {"__OS_MACOSX__","__LITTLE_ENDIAN__"}
end

