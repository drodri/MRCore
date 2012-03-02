
IF(WIN32)
ADD_DEFINITIONS(-DFREEGLUT_STATIC)
INCLUDE_DIRECTORIES(../freeglut/include)
ENDIF(WIN32)

ADD_EXECUTABLE(simulator simulator.cpp GlutApp.cpp)
ADD_EXECUTABLE(teleop teleop.cpp GlutApp.cpp)

IF(WIN32)
	TARGET_LINK_LIBRARIES(simulator mrcore freeglut_static)
	TARGET_LINK_LIBRARIES(teleop mrcore freeglut_static)
ELSE(WIN32) #LINUX	
	TARGET_LINK_LIBRARIES(simulator mrcore ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} )
ENDIF(WIN32)
		




