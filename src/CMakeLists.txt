#SOURCES SELECTION

SET(curdir ${CMAKE_CURRENT_SOURCE_DIR})

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
    MESSAGE( STATUS "CHILDREN " ${child} )
ENDIF()
ENDFOREACH()


# MR library
IF (WIN32)	
	ADD_SUBDIRECTORY(freeglut)
	ADD_LIBRARY(mrcore STATIC ${SOURCES} ${HEADERS} )
	SET_TARGET_PROPERTIES(mrcore PROPERTIES DEBUG_POSTFIX "_d")
	TARGET_LINK_LIBRARIES(mrcore ${OPENGL_LIBRARIES})
ELSE(WIN32)
	ADD_LIBRARY(mrcore SHARED ${SOURCES})
	TARGET_LINK_LIBRARIES(mrcore ${OPENGL_LIBRARIES} rt)
ENDIF(WIN32)



# Tests
	ADD_SUBDIRECTORY(tests)
	ADD_SUBDIRECTORY(apps)
	
####################################################################################################
#
#			INSTALL
#
####################################################################################################

IF (WIN32)
	SET(CMAKE_INSTALL_PREFIX C:/usr/)
ELSE(WIN32)
	SET(CMAKE_INSTALL_PREFIX /usr/local/) 
ENDIF(WIN32)

IF(WIN32)
	INSTALL(TARGETS mrcore DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
	MESSAGE(STATUS "mrcore static lib will be installed on ${CMAKE_INSTALL_PREFIX}/lib")
ELSE(WIN32)
	INSTALL(TARGETS mrcore DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
	MESSAGE(STATUS "mrcore dynamic lib will be installed on ${CMAKE_INSTALL_PREFIX}/lib")
ENDIF(WIN32)

INSTALL(FILES ${root_headers} DESTINATION	${CMAKE_INSTALL_PREFIX}/include/mrcore)
INSTALL(FILES ${base_headers} DESTINATION 	${CMAKE_INSTALL_PREFIX}/include/mrcore/base)
INSTALL(FILES ${system_headers} DESTINATION	${CMAKE_INSTALL_PREFIX}/include/mrcore/system)
INSTALL(FILES ${data_headers} DESTINATION 	${CMAKE_INSTALL_PREFIX}/include/mrcore/data)
INSTALL(FILES ${sim_headers} DESTINATION 	${CMAKE_INSTALL_PREFIX}/include/mrcore/simulation)
INSTALL(FILES ${math_headers} DESTINATION 	${CMAKE_INSTALL_PREFIX}/include/mrcore/math)
INSTALL(FILES ${world_headers} DESTINATION 	${CMAKE_INSTALL_PREFIX}/include/mrcore/world)
INSTALL(FILES ${hw_headers0} DESTINATION 	${CMAKE_INSTALL_PREFIX}/include/mrcore/hw)
INSTALL(FILES ${hw_headers1} DESTINATION 	${CMAKE_INSTALL_PREFIX}/include/mrcore/hw/wheeledbase)
INSTALL(FILES ${hw_headers2} DESTINATION 	${CMAKE_INSTALL_PREFIX}/include/mrcore/hw/lasersensor)
INSTALL(FILES ${hw_headers3} DESTINATION 	${CMAKE_INSTALL_PREFIX}/include/mrcore/hw/lasersensor3d)
INSTALL(FILES ${hw_headers4} DESTINATION 	${CMAKE_INSTALL_PREFIX}/include/mrcore/hw/lms200)
INSTALL(FILES ${hw_headers5} DESTINATION	${CMAKE_INSTALL_PREFIX}/include/mrcore/hw/powercube70)
INSTALL(FILES ${hw_headers6} DESTINATION	${CMAKE_INSTALL_PREFIX}/include/mrcore/hw/voicetts)
INSTALL(FILES ${hw_headers7} DESTINATION	${CMAKE_INSTALL_PREFIX}/include/mrcore/hw/quadrotor)
INSTALL(FILES ${hw_headers8} DESTINATION	${CMAKE_INSTALL_PREFIX}/include/mrcore/hw/camera)
INSTALL(FILES ${hw_headers9} DESTINATION	${CMAKE_INSTALL_PREFIX}/include/mrcore/hw/kinect)
INSTALL(FILES ${gl_headers} DESTINATION 	${CMAKE_INSTALL_PREFIX}/include/mrcore/gl)
INSTALL(FILES ${net_headers} DESTINATION 	${CMAKE_INSTALL_PREFIX}/include/mrcore/net)
INSTALL(FILES ${navigation2d_headers} DESTINATION 	${CMAKE_INSTALL_PREFIX}/include/mrcore/navigation2d)
INSTALL(FILES ${graphs_headers} DESTINATION 	${CMAKE_INSTALL_PREFIX}/include/mrcore/graphs)




