

INCLUDE_DIRECTORIES(
	../../btgui
	../../vendor/rbdl/include
	../../vendor/rbdl/include/rbdl
)

SET(App_SimpleDemoSelector_SRCS
	main.cpp
	BulletDemoEntries.h
	ArticulatedHierarchy.cc
	ArticulatedHierarchy.h
	CollisionShapes.h
	rbdlDemo.cpp
	rbdlDemo.h
	Simulation.cc
	Simulation.h
	string_utils.h
)

LINK_LIBRARIES(
        rbdl_static Bullet3AppSupport Bullet3Common OpenGLWindow gwen ${OPENGL_gl_LIBRARY} 
)

IF (WIN32)
	SET(App_SimpleDemoSelector_SRCS ${App_SimpleDemoSelector_SRCS}  ${App_SimpleDemoSelector_Common_SRCS})
	INCLUDE_DIRECTORIES(
				../../btgui/OpenGLWindow/GlewWindows
				../../btgui/OpenGLWindow/GlewWindows
	)
	ADD_DEFINITIONS(-DGLEW_STATIC)
ELSE(WIN32)
	IF(APPLE)
                find_library(COCOA NAMES Cocoa)
                MESSAGE(${COCOA})
                link_libraries(${COCOA})
        ELSE(APPLE)
		ADD_DEFINITIONS("-DGLEW_INIT_OPENGL11_FUNCTIONS=1")
		ADD_DEFINITIONS("-DGLEW_STATIC")
	        ADD_DEFINITIONS("-DGLEW_DYNAMIC_LOAD_ALL_GLX_FUNCTIONS=1")
		INCLUDE_DIRECTORIES(
                                ../../btgui/OpenGLWindow/GlewWindows
	        )
		LINK_LIBRARIES( pthread dl)
	ENDIF(APPLE)
ENDIF(WIN32)
 
	


ADD_EXECUTABLE(App_SimpleDemoSelector
		${App_SimpleDemoSelector_SRCS}
)
	

IF (INTERNAL_ADD_POSTFIX_EXECUTABLE_NAMES)
			SET_TARGET_PROPERTIES(App_SimpleDemoSelector PROPERTIES  DEBUG_POSTFIX "_Debug")
			SET_TARGET_PROPERTIES(App_SimpleDemoSelector PROPERTIES  MINSIZEREL_POSTFIX "_MinsizeRel")
			SET_TARGET_PROPERTIES(App_SimpleDemoSelector PROPERTIES  RELWITHDEBINFO_POSTFIX "_RelWithDebugInfo")
ENDIF(INTERNAL_ADD_POSTFIX_EXECUTABLE_NAMES)
