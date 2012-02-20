MESSAGE(STATUS "Looking for dependencies")
MESSAGE(STATUS "---------------------------------------")

#BOOST
#INCLUDE(${MRCORE_CMAKE_MODULE_PATH}/FindBoost.cmake)
#IF(BOOST_FOUND)
#	MESSAGE(STATUS "Boost support is available")
#	MESSAGE(STATUS "Boost include dir: ${BOOST_INCLUDE_DIR}")
#	INCLUDE_DIRECTORIES(${BOOST_INCLUDE_DIR})
#ELSE(BOOST_FOUND)
#	MESSAGE(STATUS "Boost NOT FOUND, but it is NOT required")
#ENDIF(BOOST_FOUND)


#EIGEN
#INCLUDE(${MRCORE_CMAKE_MODULE_PATH}/FindEigen.cmake)
#IF(EIGEN_FOUND)
#	MESSAGE(STATUS "Eigen support is available")
#	MESSAGE(STATUS "Eigen include dir: ${EIGEN_INCLUDE_DIR}")
#	INCLUDE_DIRECTORIES(${EIGEN_INCLUDE_DIR})
#ELSE(EIGEN_FOUND)
#	MESSAGE(STATUS "Eigen NOT FOUND, but it is NOT required")
#ENDIF(EIGEN_FOUND)


#OPENGL
INCLUDE(FindOpenGL)
IF(OPENGL_FOUND)
	MESSAGE(STATUS "OpenGL support is available")
	MESSAGE(STATUS "OpenGL Libraries: " ${OPENGL_LIBRARIES})
	INCLUDE_DIRECTORIES(OPENGL_INCLUDE_DIR)
ELSE(OPENGL_FOUND)
	MESSAGE(ERROR "OpenGL NOT FOUND, but it is required. Please install")
ENDIF(OPENGL_FOUND)



#################### GLUT ####################
IF(WIN32)
	#FILE(GLOB_RECURSE MyTest  "C:/*glut.h")
	#MESSAGE(STATUS "GLUT header found " ${MyTest})
	#IF(${MyTest})
	#	SET(GLUT_FOUND 1)
	#ENDIF(${MyTest})
ENDIF(WIN32)

INCLUDE(${MRCORE_CMAKE_MODULE_PATH}/FindGlut.cmake)
	IF(GLUT_FOUND)
		MESSAGE(STATUS "GLUT support is available")
		MESSAGE(STATUS "Glut Header: " ${GLUT_INCLUDE_DIR})
		MESSAGE(STATUS "Glut Libraries: " ${GLUT_LIBRARIES})
		INCLUDE_DIRECTORIES(GLUT_INCLUDE_DIR)
	ELSE(GLUT_FOUND)
		MESSAGE(STATUS "GLUT support is NOT available")
	ENDIF(GLUT_FOUND)
MESSAGE(STATUS "--------------------------------------")

#Timing functions
IF (NOT WIN32)
	# does this system provide time functions?
include (${MRCORE_CMAKE_MODULE_PATH}/CheckFunctionExists.cmake)
	MESSAGE(STATUS "Checking time functions")
	check_function_exists(gettimeofday HAVE_GETTIMEOFDAY)
	check_function_exists(clock_gettime HAVE_CLOCK_GETTIME)
ENDIF(NOT WIN32)

#LIB JPEG
INCLUDE(FindJPEG)
IF(JPEG_FOUND)
	MESSAGE(STATUS "JPEG support is available")
	MESSAGE(STATUS "JPEG Libraries: " ${JPEG_LIBRARIES})
	#MESSAGE(STATUS "JPEG Include dir: " ${JPEG_INCLUDE_DIR})
	INCLUDE_DIRECTORIES(JPEG_INCLUDE_DIR)
ELSE(JPEG_FOUND)
	MESSAGE(STATUS "JPEG NOT FOUND, but it is NOT required.")
ENDIF(JPEG_FOUND)
