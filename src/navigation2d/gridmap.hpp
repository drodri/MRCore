/**********************************************************************
 *
 * This code is part of the MRcore projec
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

template <class T>
ostream& operator << (ostream& os,const GridMap<T>& gridmap)
{
	int buffer_size=gridmap.width*sizeof(T)*gridmap.height*sizeof(T);
	os<<gridmap.width<<" "<<gridmap.height<<" "<<gridmap.resolution;
	os<<" "<<gridmap.mapX<<" "<<gridmap.mapY<<" ";
	for (int i=0;i<buffer_size;i++)
		os<<gridmap.buffer[i]<<" ";
		
	return os;
}

template <class T>
GridMap<T>::GridMap(double real_width, double real_height, double resolution, double mapX, double mapY) {
	texture=0;
	buffer=NULL;
	grid=NULL;
	this->resolution=resolution;
	this->mapX=mapX;
	this->mapY=mapY;
	this->width=(int)(real_width/resolution);
	this->height=(int)(real_height/resolution);
	allocate(this->width,this->height);
}

template <class T>
void GridMap<T>::allocate(int w,int h)
{
	if (buffer!=NULL) free();
	this->buffer_size=w*h;
	
	buffer=new T[buffer_size];
	grid=new T**[w];
	
	for (int i=0; i<w; i++){
		grid[i]=new T*[h];
		for (int j=0; j<h; j++){
			grid[i][j]=&buffer[(i+j*w)];
		}
	}

	width=w;
	height=h;
	
}

template <class T>
void GridMap<T>::free()
{
	delete[] buffer;
	for (int i=0; i<width; i++){
		delete [] grid[i];
	}
	
	delete[] grid;
	
	buffer=NULL;
	grid=NULL;
}


template <class T>
typename GridMap<T>::Ray GridMap<T>::getRay(double x, double y, double angle, double distance){
	Ray ray;
	ray.clear();
	
	double x_inc = resolution*cos(angle);
	double y_inc =  resolution* sin(angle);

	Vector2D startingPoint(x,y);
	
	int startCellValueX, startCellValueY;
	if (!WorldToGrid(startingPoint.x,startingPoint.y,startCellValueX,startCellValueY)) return ray;
	ray.push_back(RayPoint(startCellValueX,startCellValueY,x,y,getCellValue(startCellValueX,startCellValueY)));
	
	Vector2D currentPoint(x,y);
	Vector2D inc_vector(x_inc, y_inc);
	int number_steps = (int) (distance / inc_vector.module());


	for (int i=0; i<number_steps; i++){
		currentPoint=startingPoint+inc_vector*i;
		int pixelX,pixelY;
		if (!WorldToGrid(currentPoint,pixelX,pixelY)) return ray;
		if (pixelX != ray.back().pixelX || pixelY != ray.back().pixelY)
			ray.push_back(RayPoint(pixelX,pixelY,currentPoint.x,currentPoint.y,getCellValue(pixelX,pixelY)));	
	}

	return ray;
}

template <class T>
typename GridMap<T>::Ray GridMap<T>::getRay(Vector2D pointi, Vector2D pointf){

	double distance = pointi.distance2(pointf);
	Vector2D vector = pointf - pointi;
	double angle = vector.argument();
	
	return getRay(pointi.x, pointi.y, angle, distance);
}

template <class T>
bool GridMap<T>::isThereCellValueInRay(double x, double y, double angle, T value, double maxdistance, RayPoint& rayPoint)
{
	double x_inc = resolution*cos(angle);
	double y_inc =  resolution* sin(angle);

	int startCellValueX, startCellValueY;
	if (!WorldToGrid(x,y,startCellValueX,startCellValueY)) return false;
	

	Vector2D startingPoint(x,y);
	Vector2D currentPoint(x,y);

	Vector2D inc_vector(x_inc, y_inc);
	int number_steps = (int) (maxdistance / inc_vector.module());

	for (int i=0; i<number_steps; i++){
		currentPoint=startingPoint+inc_vector*i;
		int pixelX,pixelY;
		if (!WorldToGrid(currentPoint,pixelX,pixelY)) return false;
		if (getCellValue(pixelX,pixelY) == value){
			rayPoint.x=currentPoint.x;
			rayPoint.y=currentPoint.y;
			rayPoint.pixelX=pixelX;
			rayPoint.pixelY=pixelY;
			return true;
		}
	}
	
	return false;
}

template <class T>
bool GridMap<T>::isThereValueSetInRay(double x, double y, double angle, set<T> valueSet, double maxdistance, RayPoint& rayPoint)
{
	double x_inc = resolution*cos(angle);
	double y_inc =  resolution* sin(angle);

	int startCellValueX, startCellValueY;
	if (!WorldToGrid(x,y,startCellValueX,startCellValueY)) return false;
	

	Vector2D startingPoint(x,y);
	Vector2D currentPoint(x,y);

	Vector2D inc_vector(x_inc, y_inc);
	int number_steps = (int) (maxdistance / inc_vector.module());

	for (int i=0; i<number_steps; i++){
		currentPoint=startingPoint+inc_vector*i;
		int pixelX,pixelY;
		if (!WorldToGrid(currentPoint,pixelX,pixelY)) return false;
		if ( valueSet.find(getCellValue(pixelX,pixelY)) != valueSet.end() ) {
			rayPoint.x=currentPoint.x;
			rayPoint.y=currentPoint.y;
			rayPoint.pixelX=pixelX;
			rayPoint.pixelY=pixelY;
			return true;
		}
	}
	
	return false;
}

template <class T>
void GridMap<T>::scale(int grid_width){
	
	if (grid_width == width) return;
	
	double real_width = this->resolution * this->width;
	double real_height = this->resolution * this->height;
	
	int new_width=grid_width;
	int new_height=(int)(new_width / real_width)*real_height;

	//CREATE THE NEW GRID AND BUFFER
	int buffer_size=new_width*new_height;
	
	T* new_buffer = new T[buffer_size];
	T** new_grid=new T*[new_width];
	
	for(int i=0;i<new_width;i++)
		new_grid[i]= &new_buffer[i*new_height];
	
	//expand in width and height
	int x,y,prev_x,prev_y,next_x,next_y;

	prev_x=0; prev_y=0;

	for (x=1;x<=width;x++){
		next_x = (double)x*(double)new_width/(double)width;
		//cout << prev_x << "->" << next_x<< endl;
		
		for (y=1;y<=height;y++){
			next_y = (double)y*(double)new_height/(double)height;

			//cout << "\t" << prev_y << "->" << next_y << endl;
			for (int i=prev_x; i<next_x; i++){
				for(int j=prev_y; j<next_y;j++){
					//ASSIGN VALUE
					
					int ix=x,iy=y;
					if(ix==width) ix=width-1;
					if(iy==height) iy=height-1;
					//cout << i << "," << j;
					//cout << "\t->\t "<< ix << "," << iy << endl;
					new_grid[i][j]=grid[ix][iy];
				}
			}

			prev_y=next_y;
			if(prev_y==new_height) prev_y=0;
		}
		prev_x=next_x;
	}
	
	this->free();
	buffer=new_buffer;
	grid=new_grid;
	width=new_width;
	height=new_height;
	resolution=real_width/this->width;
	updated=true;

}

template <class T>
void GridMap<T>::drawGL(){

	if (texture==0) glGenTextures(1, &texture);
		
	if(updated){
		updated = false;
		
		Image* img = this->convertToImage();

		int i_height=img->getHeight();
		int i_width=img->getWidth();

		if (i_width%2 !=0) i_width-=1;
		img->scale(i_width);

		unsigned char* rgb_buffer = img->getBufferClone();
		delete img;

		//glEnable(GL_TEXTURE_2D);
		
		glBindTexture(GL_TEXTURE_2D,texture);
		glPixelStoref(GL_UNPACK_ALIGNMENT, 1);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		
		//gluBuild2DMipmaps( GL_TEXTURE_2D, 3, i_width, i_height, GL_RGB, GL_UNSIGNED_BYTE, rgb_buffer );
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB , i_width,  i_height, 0, GL_RGB, GL_UNSIGNED_BYTE, rgb_buffer);

		delete[] rgb_buffer;
	}
	
	///DRAW THE TEXTURE
	
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, texture );
	
	glPushMatrix();
	//Map drawn as a texture

	glBegin( GL_QUADS );
	glTexCoord2d(0.0,0.0); glVertex2d(mapX,mapY+height*resolution);
	glTexCoord2d(1.0,0.0); glVertex2d(mapX+width*resolution,mapY+height*resolution);
	glTexCoord2d(1.0,1.0); glVertex2d(mapX+width*resolution,mapY);
	glTexCoord2d(0.0,1.0); glVertex2d(mapX,mapY);
	glEnd();

	glDisable(GL_TEXTURE_2D);	
	glPopMatrix(); //End drawing map
	
}
