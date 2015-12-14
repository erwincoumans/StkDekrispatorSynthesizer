	
		project "App_Ragamatic"

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
		
if os.is("Linux") then initX11() end

if os.is("MacOSX") then
	links{"Cocoa.framework"}
	links{"CoreAudio.framework", "coreMIDI.framework", "Cocoa.framework"}

	defines {"__OS_MACOSX__","__LITTLE_ENDIAN__"}
end

