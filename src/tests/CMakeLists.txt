

IF (GLUT_FOUND) 
	IF(OPENGL_FOUND)
		#ADD_EXECUTABLE(glutTestMHGtransformation gluttestMHGtransformation.cpp)
		ADD_EXECUTABLE(primerEntornoGlut primerentornoglut.cpp)
		IF(WIN32)
		#	TARGET_LINK_LIBRARIES(glutTestMHGtransformation mrcore ${GLUT_LIBRARIES})
			TARGET_LINK_LIBRARIES(primerEntornoGlut mrcore  ${GLUT_LIBRARIES})
		ELSE(WIN32) #LINUX	
		#	TARGET_LINK_LIBRARIES(glutTestMHGtransformation mrcore ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} )
			TARGET_LINK_LIBRARIES(primerEntornoGlut mrcore ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} )
		ENDIF(WIN32)
		
		ENDIF(OPENGL_FOUND)
ENDIF (GLUT_FOUND)



