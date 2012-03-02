cmake_minimum_required (VERSION 2.6)

PROJECT(MRcore)

IF(WIN32)
	#in windows, it is defined by the project configuration "Release, Debug, etc"
ELSE(WIN32)
	#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -ggdb")
	SET(CMAKE_BUILD_TYPE distribution)
	SET(CMAKE_BUILD_TYPE "Release")
ENDIF(WIN32)



SET(MRCORE_CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/build_tools/cmake")
SET(MRCORE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

SET(LIBRARY_OUTPUT_PATH ${MRCORE_DIR}/lib)
SET(EXECUTABLE_OUTPUT_PATH ${MRCORE_DIR}/bin)


SET(MRCORE_INCLUDE_DIR ${MRCORE_DIR}/src/)
INCLUDE_DIRECTORIES(${MRCORE_INCLUDE_DIR})

#Dependencias
INCLUDE(${MRCORE_CMAKE_MODULE_PATH}/deps.cmake)

#################################################
#### Windows work-arounds for the dependencies ##
#FIXME

IF(WIN32)
	#Glut
	#IF(GLUT_FOUND)
	#	MESSAGE(STATUS "Your are in Windows. And Glut was found with FindGlut.cmake")
	#ELSE(GLUT_FOUND)
	#	MESSAGE(STATUS "Your are in Windows. And Glut was NOT found with FindGlut.cmake. I will make it around")
	#	SET(GLUT_FOUND 1)
	#	SET(GLUT_LIBRARIES glut32)
	#ENDIF(GLUT_FOUND)

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
IF(WIN32)
	IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  		SET(CMAKE_INSTALL_PREFIX "C:/usr" CACHE PATH "FOO install prefix" FORCE)
	ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	MESSAGE(STATUS "Default install directory is: " ${CMAKE_INSTALL_PREFIX}) 
ENDIF(WIN32)

# If manual.cmake is older than its template, there is something wrong.
#IF(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/manual.cmake")
	#IF("${CMAKE_CURRENT_SOURCE_DIR}/manual.cmake.template" IS_NEWER_THAN "${CMAKE_CURRENT_BINARY_DIR}/manual.cmake")
	#	MESSAGE(FATAL_ERROR "\n\nIt appears that manual.cmake.template is newer than your personal manual.cmake. \n"
	#	"Now you have FOUR choices:\n"
	#	"1) if you never made any changes in your personal manual.cmake, just cp manual.cmake.template manual.cmake;\n"
	#	"2) do as above and then make again your personal changes in manual.cmake;\n"
	#	"3) check the differences between manual.cmake.template and your personal manual.cmake and update the latter\n"
	#	"4) touch your personal manual.cmake (do this at your own risk, if someone changed something in the template, you will not use it).\n\n")
	#ENDIF("${CMAKE_CURRENT_SOURCE_DIR}/manual.cmake.template" IS_NEWER_THAN "${CMAKE_CURRENT_BINARY_DIR}/manual.cmake")
#	MESSAGE(STATUS "Using manual.cmake")
#ELSE(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/manual.cmake")
#	MESSAGE(STATUS "Creating manual.cmake")
#	CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/manual.cmake.template" "${CMAKE_CURRENT_BINARY_DIR}/manual.cmake" COPYONLY)
#ENDIF(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/manual.cmake")
#INCLUDE("${CMAKE_CURRENT_BINARY_DIR}/manual.cmake")



# Creation of config.h
CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/src/config.h.cmake" "${CMAKE_CURRENT_SOURCE_DIR}/src/config.h")



  
ADD_SUBDIRECTORY(src)


########### for uninstalling

# uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
