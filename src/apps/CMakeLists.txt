


ADD_EXECUTABLE(simulator simulator.cpp GlutApp.cpp)
ADD_EXECUTABLE(teleop teleop.cpp GlutApp.cpp)

IF(WIN32)
	TARGET_LINK_LIBRARIES(simulator mrcore  ${GLUT_LIBRARIES})
	TARGET_LINK_LIBRARIES(teleop mrcore  ${GLUT_LIBRARIES})
ELSE(WIN32) #LINUX	
	ARGET_LINK_LIBRARIES(simulator mrcore ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} )
ENDIF(WIN32)
		




