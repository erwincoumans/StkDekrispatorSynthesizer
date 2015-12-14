	
		project "App_Dekrispator"

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
		"*.c",
		"**.h",
		}
		
if os.is("Linux") then initX11() 
                defines  {"__OS_LINUX__","__LINUX_ALSA__","__LITTLE_ENDIAN__"}
	links {"asound","pthread"}
end

if os.is("MacOSX") then
	links{"Cocoa.framework"}
	links{"CoreAudio.framework", "coreMIDI.framework", "Cocoa.framework"}

	defines {"__OS_MACOSX__","__LITTLE_ENDIAN__"}
end

