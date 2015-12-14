
	project "Test_Gwen_OpenGL"
		
	kind "ConsoleApp"
	flags {"Unicode"}
	
	defines { "GWEN_COMPILE_STATIC" , "_HAS_EXCEPTIONS=0", "_STATIC_CPPLIB" }
	defines { "DONT_USE_GLUT"}
	
	targetdir "../../bin"
	
	includedirs 
	{
	
		"../../btgui",
		".",
	}

	initOpenGL()
	initGlew()
			
	links {
		"gwen",
	}
	
	
	files {
		"../../btgui/OpenGLWindow/OpenSans.cpp",
		"../../btgui/OpenGLWindow/TwFonts.cpp",
		"../../btgui/OpenGLWindow/TwFonts.h",
		"../../btgui/OpenGLWindow/LoadShader.cpp",
		"../../btgui/OpenGLWindow/LoadShader.h",
		"../../btgui/OpenGLWindow/GLPrimitiveRenderer.cpp",
		"../../btgui/OpenGLWindow/GLPrimitiveRenderer.h",				
		"../../btgui/OpenGLWindow/GwenOpenGL3CoreRenderer.h",
		"../../btgui/OpenGLWindow/fontstash.cpp",
		"../../btgui/OpenGLWindow/fontstash.h",
		"../../btgui/OpenGLWindow/opengl_fontstashcallbacks.cpp",
 		"../../btgui/OpenGLWindow/opengl_fontstashcallbacks.h",
		"../../btgui/Bullet3AppSupport/b3Clock.cpp",
		"../../btgui/Bullet3AppSupport/b3Clock.h",
		"**.cpp",
		"**.h",
	}
	if os.is("Windows") then
	files {
		"../../btgui/OpenGLWindow/Win32OpenGLWindow.cpp",
		"../../btgui/OpenGLWindow/Win32OpenGLWindow.h",
    "../../btgui/OpenGLWindow/Win32Window.cpp",
    "../../btgui/OpenGLWindow/Win32Window.h",
	}
	end
	if os.is("Linux") then 
		initX11()
		files{
		"../../btgui/OpenGLWindow/X11OpenGLWindow.h",
		"../../btgui/OpenGLWindow/X11OpenGLWindow.cpp"
		}
		links{"pthread"}
	end
	if os.is("MacOSX") then
		links{"Cocoa.framework"}
print("hello!")
		files{
		"../../btgui/OpenGLWindow/MacOpenGLWindow.mm",
		"../../btgui/OpenGLWindow/MacOpenGLWindow.h",
		}
	end
