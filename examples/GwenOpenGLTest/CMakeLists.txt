

INCLUDE_DIRECTORIES(
	../../btgui
)
 
 
SET(AppGwenOpenGLTest_SRCS
	Button.cpp
	Checkbox.cpp
	ComboBox.cpp
	CrossSplitter.cpp
	GroupBox.cpp
	ImagePanel.cpp
	Label.cpp
	ListBox.cpp
	MenuStrip.cpp
	Numeric.cpp
	OpenGLSample.cpp
	PanelListPanel.cpp
	ProgressBar.cpp
	Properties.cpp
	RadioButton.cpp
	ScrollControl.cpp
	Slider.cpp
	StatusBar.cpp
	TabControl.cpp
	TextBox.cpp
	TreeControl.cpp
	UnitTest.cpp
)

LINK_LIBRARIES(
        gwen OpenGLWindow Bullet3Common ${OPENGL_gl_LIBRARY} 
)

IF (WIN32)
	SET(AppGwenOpenGLTest_SRCS ${AppGwenOpenGLTest_SRCS}  ${AppGwenOpenGLTest_Common_SRCS})
	INCLUDE_DIRECTORIES(
				../../btgui/OpenGLWindow/GlewWindows
	)
	ADD_DEFINITIONS(-DGLEW_STATIC)
ELSE(WIN32)
	IF(APPLE)
		find_library(COCOA NAMES Cocoa)
		MESSAGE(${COCOA})
		link_libraries(${COCOA})
	ELSE(APPLE)

                 INCLUDE_DIRECTORIES(
                                ../../btgui/OpenGLWindow/GlewWindows
                 )
                ADD_DEFINITIONS("-DGLEW_INIT_OPENGL11_FUNCTIONS=1")
                ADD_DEFINITIONS("-DGLEW_STATIC")
                ADD_DEFINITIONS("-DGLEW_DYNAMIC_LOAD_ALL_GLX_FUNCTIONS=1")

		LINK_LIBRARIES( X11 pthread dl Xext)
	ENDIF(APPLE)
ENDIF(WIN32)


ADD_EXECUTABLE(AppGwenOpenGLTest
		${AppGwenOpenGLTest_SRCS}
)
	

IF (INTERNAL_ADD_POSTFIX_EXECUTABLE_NAMES)
			SET_TARGET_PROPERTIES(AppGwenOpenGLTest PROPERTIES  DEBUG_POSTFIX "_Debug")
			SET_TARGET_PROPERTIES(AppGwenOpenGLTest PROPERTIES  MINSIZEREL_POSTFIX "_MinsizeRel")
			SET_TARGET_PROPERTIES(AppGwenOpenGLTest PROPERTIES  RELWITHDEBINFO_POSTFIX "_RelWithDebugInfo")
ENDIF(INTERNAL_ADD_POSTFIX_EXECUTABLE_NAMES)
