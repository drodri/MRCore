/**********************************************************************
 *
 * This code is part of the MRcore project
 * Author:  Alberto Valero
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
#ifndef __MRCORE__IMAGE_H
#define __MRCORE__IMAGE_H

//#include <cstdio>
#include <iostream>
#include <cstring>

#include <string>
#include <fstream>
#include "data.h"

using namespace std;

namespace mr{




/**
 * Class Image for storing RGB (0-255) Images
 * Origin of the image is top left corner.
 * x grows going down.
 * y grows going right.
 */
class Image :public Data{

//Data Type defs
	typedef unsigned int WORD;
	typedef long LONG;
	typedef unsigned long DWORD;
	typedef unsigned char BYTE;   // 8-bit unsigned entity.

	DECLARE_MR_OBJECT(Image)

	public:
		Image(int w=0, int h=0, bool alphaChannel=false, BYTE* buf=NULL);
		~Image(){free();}

		virtual void writeToStream(Stream& stream);
		virtual void readFromStream(Stream& stream);
		void drawGL(){}

		//set of rgb colors
		typedef BYTE RGB[3];
		
		const static RGB BLACK;
		const static RGB WHITE;
		const static RGB RED;
		const static RGB GREEN;
		const static RGB BLUE;

		inline int getWidth() const {return width;}
		inline int getHeight() const {return height;}
		inline int getChannels() const {return channels;}
		inline bool hasAlphaChannel() const {return (4==channels);}
		
		inline void setPixel(int x,int y,int component, BYTE value){image[x][y][component]=value;}
		inline void setPixel(int x, int y, const BYTE* value){image[x][y][0] = value[0]; image[x][y][1] = value[1]; image[x][y][2] = value[2];}
		
		inline void setRed(int x, int y, BYTE value){setPixel(x,y,0,value);}
		inline void setGreen(int x, int y, BYTE value){setPixel(x,y,1,value);}
		inline void setBlue(int x, int y, BYTE value){setPixel(x,y,2,value);}
		inline void setAlpha(int x, int y, BYTE alpha){if(channels==4) setPixel(x,y,3,alpha);}
		
		
		inline BYTE* getPixel(int x, int y) const { return image[x][y]; }
		inline int getPixel(int x,int y,int component) const { return (int)image[x][y][component];}
		
		inline int getRed(int x, int y) const { return (int)getPixel(x,y,0); }
		inline int getGreen(int x, int y) const { return (int)getPixel(x,y,1); }
		inline int getBlue(int x, int y) const { return (int)getPixel(x,y,2); }
		inline int getAlpha(int x, int y) const { if(channels==4)return (int)getPixel(x,y,3);else return 0; }

		/**
		 * Returns a copy of the buffer. It stores the buffer size in the param size
		 * This copy can be used for OpenGL textures.
		 */
		inline BYTE* getBufferClone(int &size) const {
			size=sizeof(BYTE)*width*sizeof(BYTE)*height*sizeof(BYTE)*channels;
			BYTE* retBuffer = new BYTE[size];
			memcpy(retBuffer,buffer,size);
			return retBuffer;
		}
		inline BYTE* getBufferClone(){
			int size; return getBufferClone(size);
		}
		/**
		 * Returns a pointer to the buffer of the image.
		 * It should be use with care, as modifying the return value you are modifying the image
		 */
		inline BYTE* getPointerToBuffer(int &size) const {
			size=sizeof(BYTE)*width*sizeof(BYTE)*height*sizeof(BYTE)*channels;
			return buffer;
		}
		
		/**
		 * Returns a pointer to the matrix storing the image data.
		 * It should be use with care, as modifying the return value you are modifying the image
		 */
		inline BYTE*** getPointerToImageMatrix(int & width, int & height, int & channels){
			width=this->width;
			height=this->height;
			channels=this->channels;
			return image;
		}
			


		/**
		 * Saves Image to file. The file type is given by the extension
		 */
		bool save(string fileName);

		bool load(string fileName);

		/**
		 * Adds an alpha channel (if there does not exist) of value alpha.
		 * If the alpha channel already exists it puts alpha as value
		 */
		void addAlphaChannel(BYTE alpha=255);
		/**
		 * Revomes the alph channel
		 */
		void removeAlphaChannel();

		/**
		 * Resizes the current image to new width keeping aspect ratio.
		 * For filling new pixeles it takes the closest pixel value.
		 * @param w New image width. Height is ajusted to keep aspect ratio.
		 */
		void scale(int w);
	
	private:
		BYTE *** image;
		BYTE* buffer;

		bool save_bmp(string filename);
		bool save_jpg(string filename, const WORD quality);

		bool load_bmp(string filename);
		bool load_jpg(string filename);
		
		void allocate(int, int, int);
		void free();
		
		int width;
		int height;
		int channels;

		/**
		* This function takes as input an array of RGB values, it's width
		* and height.
		* The buffer gets then transformed to an array that can be used
		* to write to a windows bitmap file. The size of the array
		* is returned in newsize, the array itself is the
		* return value of the function.
		* Both input and output buffers must be deleted by the
		* calling function.

		* The input buffer is expected to consist of width * height
		* RGB triplets. Thus the total size of the buffer is taken as
		* width * height * 3.

		* The function then transforms this buffer so that it can be written 
		* to a windows bitmap file:
		* First the RGB triplets are converted to BGR.
		* Then the buffer is swapped around since .bmps store
		* images uside-down.
		* Finally the buffer gets DWORD ( 32bit ) aligned, 
		* meaning that each scanline ( 3 * width bytes ) gets
		* padded with 0x00 bytes up to the next DWORD boundary
		*/
		BYTE* ConvertRGBToBMPbuffer (LONG* newsize );




};


} //end namespace

#endif
