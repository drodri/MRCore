/**********************************************************************
 *
 * This code is part of the MRcore projec
 * Author:  -----------anyone
 *
 * MRcore is licenced under the Common Creative License,
 * Attribution-NonCommercial-ShareAlike 3.0
 *
 * You are free:
 *   - to Share - to copy, distribute and transmit the work
 *   - to Remix - to adapt the work
 *
 * Under the following conditions:
 *   - Attribution. You must attribute the work in the manner specified
 *     by the author or licensor (but not in any way that suggests that
 *     they endorse you or your use of the work).
 *   - Noncommercial. You may not use this work for commercial purposes.
 *   - Share Alike. If you alter, transform, or build upon this work,
 *     you may distribute the resulting work only under the same or
 *     similar license to this one.
 *
 * Any of the above conditions can be waived if you get permission
 * from the copyright holder.  Nothing in this license impairs or
 * restricts the author's moral rights.
 *
 * It is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied 
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  
 **********************************************************************/

#pragma once

#ifdef WIN32
#define _WINSOCKAPI_ //prevent the inclusion of sockets
#include <windows.h> //required for GL in Windows
#include <wingdi.h>
#include <gl/gl.h>
#include <gl/glu.h> 
#else     
#include <GL/gl.h>
#include <GL/glu.h>
#endif  


#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include <vector>

using namespace std;

namespace mr
{
enum {WHITE,RED,GREEN,BLUE,MAGENTA,CYAN,YELLOW,ORANGE,PURPLE,PINK,GREY,BLACK};

//some gl functions redefinitions
inline void glVertex3f(double _x, double _y, double _z){::glVertex3f((GLfloat)_x,(GLfloat)_y,(GLfloat)_z);}
inline void glColor4f(double _r, double _g, double _b, double _a){::glColor4f((GLfloat)_r,(GLfloat)_g,(GLfloat)_b,(GLfloat)_a);}
inline void glTranslatef(double _x, double _y, double _z){::glTranslatef((GLfloat)_x,(GLfloat)_y,(GLfloat)_z);}
inline void glRotatef(double _angle, double _x, double _y, double _z){::glRotatef((GLfloat)_angle,(GLfloat)_x,(GLfloat)_y,(GLfloat)_z);}
class GLTools 
{
public:
//Color utilities
	static void Color(int color,float transparency=1.0f);
	static void BackgroundColor(int color);	
//Textures utilities
	static unsigned int LoadTexture(string filename);
	static void DeleteTextures();
//Text utilities
	static void Print(string msg, float x=0, float y=0,float z=0);
	
//Primitives
	static void DrawEllipse(float xo,float yo,float tho,float A,float B, float C,float scale=1);///Ax2+Bxy+Cy2
	static void DrawFrame();
	static void DrawGrid(int num_cells,float size,int color=MAGENTA);
protected:
	static std::vector<string> textures_names;
	static std::vector<unsigned int> textures_ids;
};

}; //end namespace mr
