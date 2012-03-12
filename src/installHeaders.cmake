FILE(READ mrcoreDirPath curdir2)

SET(curdir2 ${curdir2}/src)
MESSAGE( "The source directory to extract headers is:" ${curdir2})

FILE(GLOB children RELATIVE ${curdir2} ${curdir2}/*)
SET(HEADERS "")
SET(HEADERS_HPP "")
FOREACH(child ${children})
IF(IS_DIRECTORY ${curdir2}/${child})
    FILE(GLOB_RECURSE child_headers ${curdir2}/${child}/*.h)
    FILE(GLOB_RECURSE child_headers_hpp ${curdir2}/${child}/*.hpp)
	SET(HEADERS ${HEADERS} ${child_headers})
	SET(HEADERS_HPP ${HEADERS_HPP} ${child_headers_hpp})
ENDIF()
ENDFOREACH()

SET(MRCORE_H "#pragma once\n")
FOREACH(header ${HEADERS})
string(REGEX REPLACE ^${curdir2}/ "" DIR_FILE ${header})
string(REGEX MATCH "(.*)[/\\]" DIR_INSTALL ${DIR_FILE})
SET(MRCORE_H ${MRCORE_H} "#include \"" ${DIR_FILE} "\"\n")
#INSTALL(FILES ${header} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/mrcore/${DIR_INSTALL})
FILE(COPY ${header} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/mrcore/${DIR_INSTALL})
ENDFOREACH()

FOREACH(header ${HEADERS_HPP})
string(REGEX REPLACE ^${curdir2}/ "" DIR_FILE ${header})
string(REGEX MATCH "(.*)[/\\]" DIR_INSTALL ${DIR_FILE})
FILE(COPY ${header} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/mrcore/${DIR_INSTALL})
ENDFOREACH()

FILE(COPY "${curdir2}/config.h" DESTINATION ${CMAKE_INSTALL_PREFIX}/include/mrcore)

###INSTALL mrcore.h
SET(MRCORE_H_FILE ${CMAKE_INSTALL_PREFIX}/include/mrcore/mrcore.h)
FILE(WRITE ${MRCORE_H_FILE} ${MRCORE_H})
