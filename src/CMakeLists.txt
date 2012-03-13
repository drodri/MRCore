#SOURCES SELECTION

SET(curdir ${CMAKE_CURRENT_SOURCE_DIR})
#SET( ENV{MRCORE_PATH} ${curdir} ) 
INCLUDE_DIRECTORIES(${curdir})

# Creation of config.h
CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake" "${CMAKE_CURRENT_SOURCE_DIR}/config.h") 

####################################################################################################
#
# Iterate over children, creating SOURCE_GROUPs 
#
####################################################################################################
FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
SET(SOURCES "") 
SET(HEADERS "")
FOREACH(child ${children})
IF(IS_DIRECTORY ${curdir}/${child})
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
ELSE(WIN32)
	ADD_LIBRARY(mrcore SHARED ${SOURCES})
	TARGET_LINK_LIBRARIES(mrcore ${OPENGL_LIBRARIES} rt)
ENDIF(WIN32)


####################################################################################################
#
#			INSTALL
#
####################################################################################################

INSTALL(TARGETS mrcore DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
MESSAGE(STATUS "mrcore lib will be installed on ${CMAKE_INSTALL_PREFIX}/lib")
INSTALL(SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/installHeaders.cmake)









