	project "stk"

	language "C++"
				
	kind "StaticLib"
		
	includedirs {"../include"}

	files {
		"*.cpp",
		"*.h"
	}
	defines {"__STK_REALTIME__"}
	
		if os.is("Windows") then
			defines {"WIN32","__WINDOWS_MM__","__LITTLE_ENDIAN__","__WINDOWS_DS__"}
		end
	 if os.is("Linux") then
                defines  {"__OS_LINUX__","__LINUX_ALSA__","__LITTLE_ENDIAN__"}
        end

	if os.is("MacOSX") then
		defines {"__OS_MACOSX__","__MACOSX_CORE__","__LITTLE_ENDIAN__"}
        end

