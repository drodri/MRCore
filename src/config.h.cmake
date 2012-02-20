#ifndef __CONFIG_MRCORE_H
#define __CONFIG_MRCORE_H

// automatic found external libraries
#cmakedefine OPENGL_FOUND
#cmakedefine GLUT_FOUND
#cmakedefine JPEG_FOUND

#cmakedefine HAVE_GETTIMEOFDAY
#cmakedefine HAVE_CLOCK_GETTIME
#ifndef WIN32
#cmakedefine WIN32
#endif
#endif
