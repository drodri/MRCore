#SOURCES SELECTION

AUX_SOURCE_DIRECTORY( base base_sources )  
AUX_SOURCE_DIRECTORY( system system_sources ) 
AUX_SOURCE_DIRECTORY( net net_sources )
AUX_SOURCE_DIRECTORY( math math_sources )
AUX_SOURCE_DIRECTORY( data data_sources )
AUX_SOURCE_DIRECTORY( world world_sources )
AUX_SOURCE_DIRECTORY( hw hw_sources )
AUX_SOURCE_DIRECTORY( simulation sim_sources)
AUX_SOURCE_DIRECTORY( navigation2d navigation2d_sources )
AUX_SOURCE_DIRECTORY( graphs graphs_sources )
FILE(GLOB_RECURSE hw_sources hw/*.cpp) 

IF (OPENGL_FOUND) 
	AUX_SOURCE_DIRECTORY( gl gl_sources )  
ENDIF(OPENGL_FOUND)

SET(SOURCES 	${base_sources}
				${system_sources}
				${net_sources}
				${hw_sources}
				${gl_sources}
				${data_sources}
				${world_sources}
				${math_sources}
				${navigation2d_sources}
				${graphs_sources}
				${sim_sources}
				)
			
# HEADER SELECTION
FILE(GLOB root_headers 		*.h)
FILE(GLOB base_headers 		base/*.h)
FILE(GLOB system_headers	system/*.h*)
FILE(GLOB net_headers		net/*.h)
FILE(GLOB graphs_headers	graphs/*.h*)
FILE(GLOB sim_headers		simulation/*.h*)  
FILE(GLOB hw_headers0		hw/*.h)
FILE(GLOB hw_headers1		hw/wheeledbase/*.h) 
FILE(GLOB hw_headers2		hw/lasersensor/*.h)
FILE(GLOB hw_headers3		hw/lasersensor3d/*.h)
FILE(GLOB hw_headers4		hw/lms200/*.h)
FILE(GLOB hw_headers5		hw/powercube70/*.h)
FILE(GLOB hw_headers6		hw/voicetts/*.h)
FILE(GLOB hw_headers7		hw/quadrotor/*.h)
FILE(GLOB hw_headers8		hw/camera/*.h)
FILE(GLOB hw_headers9		hw/kinect/*.h)
SET(hw_headers	${hw_headers0} ${hw_headers1} ${hw_headers2} 
				${hw_headers3} ${hw_headers4} ${hw_headers5} ${hw_headers6} 
				${hw_headers7} ${hw_headers8} ${hw_headers9} )
FILE(GLOB math_headers 		math/*.h)
FILE(GLOB world_headers 	world/*.h)
FILE(GLOB gl_headers 		gl/*.h)
FILE(GLOB data_headers 		data/*.h)
FILE(GLOB navigation2d_headers navigation2d/*.h*)


SET(HEADERS	${base_headers} 
				${system_headers}
				${net_headers}
				${hw_headers}
				${gl_headers}
				${data_headers}
				${world_headers}
				${math_headers}
				${navigation2d_headers}
				${graphs_headers}
				${sim_headers}
				)

SOURCE_GROUP(base FILES ${base_sources} ${base_headers})
SOURCE_GROUP(system FILES ${system_sources} ${system_headers})
SOURCE_GROUP(net FILES ${net_sources} ${net_headers})
SOURCE_GROUP(data FILES ${data_sources} ${data_headers})
SOURCE_GROUP(math FILES ${math_sources} ${math_headers} )
SOURCE_GROUP(hw FILES ${hw_sources} ${hw_headers})
SOURCE_GROUP(gl FILES ${gl_sources} ${gl_headers})
SOURCE_GROUP(world FILES ${world_sources} ${world_headers})
SOURCE_GROUP(navigation2d FILES ${navigation2d_sources} ${navigation2d_headers})
SOURCE_GROUP(graphs FILES ${graphs_sources} ${graphs_headers})
SOURCE_GROUP(simulation FILES ${sim_sources} ${sim_headers})


IF (WIN32)
	SET(CMAKE_INSTALL_PREFIX C:/usr/)
ELSE(WIN32)
	SET(CMAKE_INSTALL_PREFIX /usr/local/) 
ENDIF(WIN32)


# MR library
IF (WIN32)	
	ADD_SUBDIRECTORY(freeglut)
	ADD_LIBRARY(mrcore STATIC ${SOURCES} ${HEADERS} )
	SET_TARGET_PROPERTIES(mrcore PROPERTIES DEBUG_POSTFIX "_d")
	TARGET_LINK_LIBRARIES(mrcore ${OPENGL_LIBRARIES})
ELSE(WIN32)
	ADD_LIBRARY(mrcore SHARED ${SOURCES})
#	ADD_LIBRARY(mrcores STATIC ${SOURCES})
	TARGET_LINK_LIBRARIES(mrcore ${OPENGL_LIBRARIES} rt)
ENDIF(WIN32)

IF(JPEG_FOUND)
	TARGET_LINK_LIBRARIES(mrcore ${JPEG_LIBRARIES})
ENDIF(JPEG_FOUND)

# Tests
	ADD_SUBDIRECTORY(tests)
	ADD_SUBDIRECTORY(apps)
	
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




