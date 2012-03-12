cmake_minimum_required (VERSION 2.6)

PROJECT(MRCore)

IF(WIN32)
	#in windows, it is defined by the project configuration "Release, Debug, etc"
ELSE(WIN32)
	#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -ggdb")
	SET(CMAKE_BUILD_TYPE distribution)
	SET(CMAKE_BUILD_TYPE "Release")
ENDIF(WIN32)

#WRITE A FILE TO STORE the source folder, so install scripts can know where it is
FILE(WRITE ${CMAKE_BINARY_DIR}/mrcoreDirPath ${CMAKE_CURRENT_SOURCE_DIR})

SET(MRCORE_CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/build_tools/cmake")
SET(MRCORE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

#Dependencias
INCLUDE(${MRCORE_CMAKE_MODULE_PATH}/deps.cmake)

#################################################
#### Windows work-arounds for the dependencies ##
#FIXME

IF(WIN32)
	#OpenGL
	IF (OPENGL_FOUND)
		MESSAGE(STATUS "Your are in Windows. And OpenGL was found with FindOPENGL.cmake")
	ELSE(OPENGL_FOUND)
		MESSAGE(STATUS "Your are in Windows. And OpenGL was NOT found with FindOPENGL.cmake. I will make it around")
		SET(OPENGL_FOUND 1)
		SET(OPENGL_LIBRARIES opengl32 glu32 )
	ENDIF(OPENGL_FOUND)
ENDIF(WIN32)
#################################################


########### To define a default install directory in Windows
IF (WIN32)
	SET(CMAKE_INSTALL_PREFIX "C:/mrcore" CACHE PATH "FOO install prefix" FORCE)
ELSE(WIN32)
	SET(CMAKE_INSTALL_PREFIX /usr/local/mrcore) 
ENDIF(WIN32)
MESSAGE(STATUS "Default install directory is: " ${CMAKE_INSTALL_PREFIX}) 

ADD_SUBDIRECTORY(src)

########### for uninstalling

# uninstall target
#configure_file(
#    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
 #   "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
 #   IMMEDIATE @ONLY)

#add_custom_target(uninstall
#    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
