#SOURCES SELECTION

SET(curdir ${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${curdir})

# Creation of config.h
CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake" "${CMAKE_CURRENT_SOURCE_DIR}/config.h")

##
### Iterate over children, creating SOURCE_GROUPs 
##
FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
SET(dirlist "")
SET(SOURCES "")
SET(HEADERS "")
FOREACH(child ${children})
IF(IS_DIRECTORY ${curdir}/${child})
    SET(dirlist ${dirlist} ${child})
    FILE(GLOB_RECURSE child_sources ${curdir}/${child}/*.cpp) 
    FILE(GLOB_RECURSE child_headers ${curdir}/${child}/*.h)
    SET(SOURCES ${SOURCES} ${child_sources})
    SET(HEADERS ${HEADERS} ${child_headers})
    SOURCE_GROUP(${child} FILES ${child_sources} ${child_headers})
ENDIF()
ENDFOREACH()


# MR library
IF (WIN32)	
	ADD_LIBRARY(mrcore STATIC ${SOURCES} ${HEADERS} )
	SET_TARGET_PROPERTIES(mrcore PROPERTIES DEBUG_POSTFIX "_d")
#	TARGET_LINK_LIBRARIES(mrcore ${OPENGL_LIBRARIES})
ELSE(WIN32)
	ADD_LIBRARY(mrcore SHARED ${SOURCES})
	TARGET_LINK_LIBRARIES(mrcore ${OPENGL_LIBRARIES} rt)
ENDIF(WIN32)


####################################################################################################
#
#			INSTALL
#
####################################################################################################
IF(WIN32)
	INSTALL(TARGETS mrcore DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
	MESSAGE(STATUS "mrcore static lib will be installed on ${CMAKE_INSTALL_PREFIX}/lib")
ELSE(WIN32)
	INSTALL(TARGETS mrcore DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
	MESSAGE(STATUS "mrcore dynamic lib will be installed on ${CMAKE_INSTALL_PREFIX}/lib")
ENDIF(WIN32)

#####INSTALL HEADERS
SET(MRCORE_H "#pragma once\n")
FOREACH(header ${HEADERS})
string(REGEX REPLACE ^${curdir}/ "" DIR_FILE ${header})
string(REGEX MATCH "(.*)[/\\]" DIR_INSTALL ${DIR_FILE})
SET(MRCORE_H ${MRCORE_H} "#include \"" ${DIR_FILE} "\"\n")
INSTALL(FILES ${header} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/mrcore/${DIR_INSTALL})
ENDFOREACH()

###INSTALL mrcore.h
FILE(WRITE ${CMAKE_INSTALL_PREFIX}/include/mrcore/mrcore.h ${MRCORE_H})





