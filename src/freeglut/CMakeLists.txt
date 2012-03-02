AUX_SOURCE_DIRECTORY( src freeglut_sources ) 
 
ADD_DEFINITIONS(-DFREEGLUT_STATIC)
INCLUDE_DIRECTORIES(include)
ADD_LIBRARY(freeglut_static STATIC
			${freeglut_sources})
			
		




