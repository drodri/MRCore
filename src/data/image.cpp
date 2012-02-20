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
#include "image.h"
#include "system/filesystem.h"
#include "base/logger.h" 
#include "config.h"
#include <assert.h>
#include <cstdio>

//lib jpeg support. If it is not installed you will not be able to save images in jpeg format
#ifdef JPEG_FOUND
	#include "jpeglib.h"
	#include "setjmp.h"
#endif

using namespace std;
namespace mr{

const Image::RGB Image::BLACK={0,0,0};
const Image::RGB Image::WHITE={255,255,255};
const Image::RGB Image::GREEN={0,255,0};
const Image::RGB Image::RED={255,0,0};
const Image::RGB Image::BLUE={0,0,255};

IMPLEMENT_MR_OBJECT(Image);


Image::Image(int w, int h, bool alpha, BYTE* buf)
{
	if(alpha) channels=4;
	else channels=3;
	buffer=NULL, image=NULL; allocate(w,h,channels);
	if(buf!=NULL){
		int size=sizeof(BYTE)*width*sizeof(BYTE)*height*sizeof(BYTE)*channels;
		memcpy(buffer,buf,size);
		//cout << "Copying buffer" << endl;
	}
}
void Image::writeToStream(Stream& stream)
{
	stream<<width<<height<<channels;
	int s=sizeof(BYTE)*width*sizeof(BYTE)*height*sizeof(BYTE)*channels;
	string aux((char*)buffer,s);
	stream<<aux;
}
void Image::readFromStream(Stream& stream)
{
	int w=0,h=0,c=0;
	stream>>w>>h>>c;
	allocate(w,h,c);

	string aux;
	stream>>aux;
	int s=sizeof(BYTE)*width*sizeof(BYTE)*height*sizeof(BYTE)*channels;
	assert(aux.length()==s);
	memcpy(buffer,aux.data(),aux.length());

}
void Image::removeAlphaChannel(){
// if it does not have alpha channel return
	if (channels==3) return;
	
	int buf_size=sizeof(BYTE)*width*sizeof(BYTE)*height*sizeof(BYTE)*4;
	int new_size=sizeof(BYTE)*width*sizeof(BYTE)*height*sizeof(BYTE)*3;;

	channels=3;
	
	BYTE* new_buffer = new BYTE[new_size];
	
	int j=0;
	for (int i=0; i<buf_size; i+=4 ){
		new_buffer[j]=buffer[i];
		new_buffer[j+1]=buffer[i+1];
		new_buffer[j+2]=buffer[i+2];
		j+=3;
	}
	
	this->free();
	buffer=new_buffer;

	image = new BYTE**[width];
	
	for (int i=0; i<width; i++){
		image[i]=new BYTE*[height];
		for (int j=0; j<height; j++){
			image[i][j]=&buffer[(i+j*width)*channels];
		}
	}
}

void Image::scale(int w){
	int new_width=w;
	int new_height=(int)( (double)new_width / (double)width * (double)height );

	//CREATE THE NEW IMAGE AND BUFFER
	int new_size=new_width*sizeof(BYTE)*new_height*sizeof(BYTE)*channels;
	BYTE* new_buffer = new BYTE[new_size];
	BYTE*** new_image;
	
	new_image = new BYTE**[new_width];
	
	for (int i=0; i<new_width; i++){
		new_image[i]=new BYTE*[new_height];
		for (int j=0; j<new_height; j++){
			new_image[i][j]=&new_buffer[(i+j*new_width)*channels];
		}
	}
	
	//expand in width and height
	int x,y,prev_x,prev_y,next_x,next_y;

	prev_x=0; prev_y=0;

	for (x=1;x<=width;x++){
		next_x = (int)((double)x*(double)new_width/(double)width);
		//cout << prev_x << "->" << next_x<< endl;
		
		for (y=1;y<=height;y++){
			next_y = (int)((double)y*(double)new_height/(double)height);

	
			//cout << "\t" << prev_y << "->" << next_y << endl;
			for (int i=prev_x; i<next_x; i++){
				for(int j=prev_y; j<next_y;j++){
					//ASSIGN VALUE
					
					int ix=x,iy=y;
					if(ix==width) ix=width-1;
					if(iy==height) iy=height-1;
					//cout << i << "," << j;
					//cout << "\t->\t "<< ix << "," << iy << endl;
					for (int c=0;c<channels;c++)
						new_image[i][j][c]=image[ix][iy][c];
				}
			}
	

			prev_y=next_y;
			if(prev_y==new_height) prev_y=0;
		}
		prev_x=next_x;
	}

	

	this->free();
	buffer=new_buffer;
	image=new_image;
	width=new_width;
	height=new_height;
	//cout << "Width " << new_width << " Height " << new_height << endl;
	
}

void Image::addAlphaChannel(BYTE alpha){
// if it has already alpha channel return
	if (channels==4){
		for (int i=0;i<width;i++){
			for (int j=0;j<height;j++){
				setAlpha(i,j,alpha);
			}
		}	
	return;
	}
	
	int buf_size=sizeof(BYTE)*width*sizeof(BYTE)*height*sizeof(BYTE)*3;
	int new_size=sizeof(BYTE)*width*sizeof(BYTE)*height*sizeof(BYTE)*4;

	channels=4;
	
	BYTE* new_buffer = new BYTE[new_size];
	int j=0;
	for (int i=0; i<buf_size; i+=3 ){
		new_buffer[j]=buffer[i];
		new_buffer[j+1]=buffer[i+1];
		new_buffer[j+2]=buffer[i+2];
		new_buffer[j+3]=alpha;
		j+=4;
	}
	
	this->free();
	buffer=new_buffer;

	image = new BYTE**[width];
	
	for (int i=0; i<width; i++){
		image[i]=new BYTE*[height];
		for (int j=0; j<height; j++){
			image[i][j]=&buffer[(i+j*width)*channels];
		}
	}
}


void Image::allocate(int w, int h, int c){
	if (buffer!=NULL){
		free();
	}
	
	buffer = new BYTE[w*h*c];
	image = new BYTE**[w];
	
	for (int i=0; i<w; i++){
		image[i]=new BYTE*[h];
		for (int j=0; j<h; j++){
			image[i][j]=&buffer[(i+j*w)*c];
		}
	}
	
	width=w;
	height=h;
	channels=c;
}

void Image::free(){
	delete [] buffer;
	for (int i=0;i<width;i++){
		delete [] image[i];
	}
	
	delete [] image;
	
	buffer=NULL;
	image=NULL;
	
}

bool Image::load(string filename){
	if ( (isExtensionOfFile(filename, "jpg") ) ||
			(isExtensionOfFile(filename, "jpeg") ) ) return load_jpg(filename);
	else if (isExtensionOfFile(filename, "bmp")  ) return load_bmp(filename);
	else LOG_ERROR("Extension not known");
	return false;
}

bool Image::load_jpg(string filename){
	LOG_ERROR("JPEG images cannot be loaded on the current implementation");
	return false;
}

bool Image::load_bmp(string filename)
{
	/*
	fstream file;
	file.open(filename.c_str(), fstream::in | fstream::binary);
	
	/* make sure the file is there and open it read-only (binary) */ 
	/*if (!file.good())
	{
		LOG_ERROR("Cannot open " << filename);
		return false;
	}
	
	//unsigned char header[64]={0};	
	char header[64]={0};	
	file.read(header,54);
	
	if(header[0]!='B' || header[1]!='M'){
		LOG_ERROR(filename << " is not a bitmap file");
		file.close();
		return false;
	} 
*/
		 
//----------------------------------------------------------------------		 
		 
	std::FILE *const file = fopen(filename.c_str(), "rb");

	if (file == NULL){
		LOG_ERROR("Cannot open " << filename);
		return false;
	}
	
	unsigned char header[64];
	fread(header,54,1,file);

	if(header[0]!='B' || header[1]!='M'){
		LOG_ERROR(filename << " is not a bitmap file");
		fclose(file);
		return false;
	} 

	/* get the position of the actual bitmap data */ 
	int file_size = header[0x02] + (header[0x03]<<8) + (header[0x04]<<16) + (header[0x05]<<24);
   int bfOffBits = header[0x0A] + (header[0x0B]<<8) + (header[0x0C]<<16) + (header[0x0D]<<24);
   int bmpWidth = header[0x12] + (header[0x13]<<8) + (header[0x14]<<16) + (header[0x15]<<24);
   int bmpHeight = header[0x16] + (header[0x17]<<8) + (header[0x18]<<16) + (header[0x19]<<24);
	short int biBiLayers = header[0x1A] + (header[0x1B]<<8);
   short int biBitCount = header[0x1C] + (header[0x1D]<<8);
	
   
	if (biBiLayers != 1){ 
		LOG_ERROR("biBiLayers is not 1. It is" << biBiLayers << ".");
		fclose(file);
		return false;
	}

	
	if (biBitCount != 24){
		LOG_ERROR("It is not an RBG image. It is " << biBitCount << ".");
		fclose(file);
		//file.close();
		return false;
	} 

	long int biSizeImage; 
	/* calculate the size of the image in bytes */ 
	biSizeImage = bmpWidth * bmpHeight * 3; 

	//create new buffer
	allocate(bmpWidth,bmpHeight,3);
	
	/* seek to the actual data */ 
	//file.seekp(bfOffBits, ios::beg);

	fseek(file, bfOffBits, SEEK_SET); 

	
	if (!fread(buffer, biSizeImage, 1, file)){
	//char * rbuffer = (char *)buffer;
	//if (!file.read(rbuffer,biSizeImage)){
		LOG_ERROR("Unable to parse the bitmap data");
		fclose(file);
		//file.close();
		return false;
	} 

	/* swap red and blue (bgr -> rgb) */ 
	for (int i = 0; i < biSizeImage; i += 3) 
	{ 
		unsigned char temp = buffer[i]; 
		buffer[i] = buffer[i + 2]; 
		buffer[i + 2] = temp; 
	}

	// swap bottom up
	
	for (int x=0; x<width; x++){
		for (int y=0;y<height/2; y++){
			for (int c=0;c<3;c++){
				unsigned char temp;
				temp=image[x][y][c];
				image[x][y][c]=image[x][height-y-1][c];				
				image[x][height-y-1][c]=temp;
			}
		}
	}
	fclose(file);
	//file.close();
	return true; 
}



bool Image::save(string fileName){

	if ( (isExtensionOfFile(fileName, "jpg") ) ||
			(isExtensionOfFile(fileName, "jpeg") ) ) return save_jpg(fileName,100);
	else if (isExtensionOfFile(fileName, "bmp")  ) return save_bmp(fileName);
	else LOG_ERROR("Extension not known");
	return false;
}

bool Image::save_jpg(string filename, const WORD quality){

	#ifdef JPEG_FOUND
	//If the image has 4 channels (alpha) let's make a RGB copy and save it instead
	if(channels==4){
		Image rgbImg(width,height,channels, buffer);
		rgbImg.removeAlphaChannel();
		return rgbImg.save_jpg(filename,quality);
	}
		
	WORD dimbuf = 3;

	J_COLOR_SPACE colortype = JCS_RGB;


	// Call libjpeg functions
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	
	
	std::FILE *const nfile = fopen(filename.c_str(),"wb");
	jpeg_stdio_dest(&cinfo,nfile);
	cinfo.image_width = width;
	cinfo.image_height = height;
	cinfo.input_components = dimbuf;
	cinfo.in_color_space = colortype;
	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo,quality<100?quality:100,TRUE);
	jpeg_start_compress(&cinfo,TRUE);

	JSAMPROW row_pointer[1];

	int line=0;
	while (cinfo.next_scanline < cinfo.image_height) {
	  *row_pointer = &buffer[line*3*width]; line++;
	  jpeg_write_scanlines(&cinfo,row_pointer,1);
	}
	
	jpeg_finish_compress(&cinfo);
	fclose(nfile);
	jpeg_destroy_compress(&cinfo);
	return true;
	#else
	LOG_ERROR("LibJPEG is not installed on your system. You cannot save in jpeg format");
	return false;
	#endif

}

bool Image::save_bmp(string filename){

	//If the image has 4 channels (alpha) let's make a RGB copy and save it instead
	if(channels==4){
		Image rgbImg(width,height,true,this->getBufferClone());
		rgbImg.removeAlphaChannel();
		return rgbImg.save_bmp(filename);
	}
	
	fstream nfile;
	nfile.open(filename.c_str(), fstream::out | fstream::binary);
	//std::FILE *const nfile = fopen(filename.c_str(),"wb");

	//unsigned char header[54] = { 0 }, align_buf[4] = { 0 };
	unsigned char align_buf[4] = { 0 };
	char header[54] = { 0 };

	const unsigned int align = 0;
	LONG buf_size;
	BYTE* bmpBuffer=ConvertRGBToBMPbuffer(&buf_size);
	char * wbmpBuffer = (char *) bmpBuffer;
	
	unsigned int file_size = 54 + buf_size;
	
	header[0] = 'B'; header[1] = 'M';
	header[0x02] = file_size&0xFF;
	header[0x03] = (file_size>>8)&0xFF;
	header[0x04] = (file_size>>16)&0xFF;
	header[0x05] = (file_size>>24)&0xFF;
	header[0x0A] = 0x36;
	header[0x0E] = 0x28;
	header[0x12] = width&0xFF;
	header[0x13] = (width>>8)&0xFF;
	header[0x14] = (width>>16)&0xFF;
	header[0x15] = (width>>24)&0xFF;
	header[0x16] = height&0xFF;
	header[0x17] = (height>>8)&0xFF;
	header[0x18] = (height>>16)&0xFF;
	header[0x19] = (height>>24)&0xFF;
	header[0x1A] = 1;
	header[0x1B] = 0;
	header[0x1C] = 24;
	header[0x1D] = 0;
	header[0x22] = buf_size&0xFF;
	header[0x23] = (buf_size>>8)&0xFF;
	header[0x24] = (buf_size>>16)&0xFF;
	header[0x25] = (buf_size>>24)&0xFF;
	header[0x27] = 0x1;
	header[0x2B] = 0x1;

	nfile.write(header,54);
	nfile.write(wbmpBuffer,buf_size);
	nfile.close();
	//fwrite(header,54,1,nfile);
	//fwrite(bmpBuffer,buf_size,1,nfile);
	//fclose(nfile);

	if (bmpBuffer != NULL )delete [] bmpBuffer;

	return true;
}


Image::BYTE* Image::ConvertRGBToBMPbuffer (LONG* newsize )
{

	// first make sure the parameters are valid
	if ( ( NULL == buffer ) || ( width == 0 ) || ( height == 0 ) )
		return NULL;

	// now we have to find with how many bytes
	// we have to pad for the next DWORD boundary	

	int padding = 0;
	int scanlinebytes = width * 3;

	while ( ( scanlinebytes + padding ) % 4 != 0 )     // DWORD = 4 bytes
		padding++;
	// get the padded scanline width
	int psw = scanlinebytes + padding;
	
	// we can already store the size of the new padded buffer
	*newsize = height * psw;

	// and create new buffer
	BYTE* newbuf = new BYTE[*newsize];


	
	// fill the buffer with zero bytes then we dont have to add
	// extra padding zero bytes later on
	memset ( newbuf, 0, *newsize );


	// now we loop trough all bytes of the original buffer, 
	// swap the R and B bytes and the scanlines
	LONG bufpos = 0;   
	LONG newpos = 0;
	for ( int y = 0; y < height; y++ )
		for ( int x = 0; x < 3 * width; x+=3 )
		{
			bufpos = y * 3 * width + x;     // position in original buffer
			newpos = ( height - y - 1 ) * psw + x;           // position in padded buffer

			newbuf[newpos] = buffer[bufpos+2];       // swap r and b
			newbuf[newpos + 1] = buffer[bufpos + 1]; // g stays
			newbuf[newpos + 2] = buffer[bufpos];     // swap b and r
		}

	return newbuf;
}
	 

} //end namespace
