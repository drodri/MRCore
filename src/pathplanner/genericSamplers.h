/**********************************************************************
 *
 * This code is part of the MRcore project
 * Author:   Miguel Hernando Gutierrez
 * 
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

#ifndef __MRCORE__GENERIC_SAMPLERS__H
#define __MRCORE__GENERIC_SAMPLERS__H


/*********************************
A set of samplers for diferential robotic platforms. The samples
are not actions but space locations. a generic 2d/3D implementation
is able to detect world dimensions where the robot can move through the bounding
boxes. 
**********************************/
#include <stdlib.h>
#include <time.h>
#include <vector>

using namespace std;
#include "../world/world.h"
#include "../world/robotsim.h"
namespace mr
{
/*!
    \class Sampler
    A incremental Low Discrepancy deterministic Lattice method for 2D sampling
	Based on the work of Lindermann and Lavalle.
	Simplest version: the resolution is the same for all axis..

	DESIGN INFO:
	
	*/





typedef vector<double> Sample;
//generic abstract class for samplers
class Sampler
{
public: //public interface
	virtual void initialize(){};
	virtual Sample getNextSample()=0;
	int getDimension(){return dimension;}
	//generic usefull functions normalized (12 measures)
	static double sampleGaussian(double mean,double cov);
	//generic usefull functions normalized (6 measures)
	static double sampleGaussian(int n, double mean,double cov);
	void setWorld(World *world);
private:
	vector<double> min, range;
	Sample sample;
	int dimension;
	//coverts the normalized sample 0...1 to the space dimension
protected:	
	//protected data. The derived samplers have to put the normalized values here 
	double *normalizedSample;
	//protected constructors
	Sampler(Vector2D min, Vector2D range);
	Sampler(Vector3D min, Vector3D range);
	Sampler(BoundingBox &box);
	Sampler(World *world);
	
	Sampler(RobotSim *r);
	void configureSampler( vector<double> _min, vector<double> _range );
	const Sample &computeSample();

	virtual ~Sampler(){delete [] normalizedSample;}
};
//random sampler: generic with two specializations for Vector3D and Vector2D
class RandomSampler : public Sampler
{
public:	
	void initialize(){srand( (unsigned)time( NULL ) );}
	RandomSampler(Vector2D min, Vector2D range):Sampler(min,range){RandomSampler::initialize();}
	RandomSampler(Vector3D min, Vector3D range):Sampler(min,range){RandomSampler::initialize();}
	RandomSampler(BoundingBox &box):Sampler(box){RandomSampler::initialize();}
	RandomSampler(World *world):Sampler(world){RandomSampler::initialize();}
	RandomSampler(RobotSim *r):Sampler(r){RandomSampler::initialize();}
	Sample getNextSample();
};


/*!
    \class UniformSampler
    A incremental Low Discrepancy deterministic Lattice method for 2D sampling
	Based on the work of Lindermann and LaValle.
	Simplest version: the resolution is the same for all axis..
	DESIGN INFO: the L sequence is obtained through Van der Corput sequence
	
	*/
class UniformSampler : public Sampler
{
public:
	UniformSampler(Vector2D min, Vector2D range):Sampler(min,range),L(0){initializeL();}
	UniformSampler(Vector3D min, Vector3D range):Sampler(min,range),L(0){initializeL();}
	UniformSampler(BoundingBox &box):Sampler(box),L(0){initializeL();}
	UniformSampler(World *world):Sampler(world),L(0){initializeL();}
	UniformSampler(RobotSim *r):Sampler(r),L(0){initializeL();}
	~UniformSampler();
	Sample getNextSample();
	void initialize(){iteration=0;}
protected:
	//specific method for initializinf the L vector
	void initializeL();
	//specific recursive method for obtaining the 
	Sample getNextNormalizedSample(int n, Sample base, double factor);
	//specific atributes. the set L: [2^n][n]
	double **L;
	int L_n;
	int iteration;
	//some L predefined cases L2, L3, L6
	static double L_2[4][2];
	static double L_3[8][3];
	static double L_6[64][6];
};
class GaussianSampler : public UniformSampler
{
public:
	void initialize(){srand( (unsigned)time( NULL ) );}
	GaussianSampler(Vector2D min, Vector2D range):UniformSampler(min,range){GaussianSampler::initialize();};
	GaussianSampler(Vector3D min, Vector3D range):UniformSampler(min,range){GaussianSampler::initialize();};
	GaussianSampler(BoundingBox &box):UniformSampler(box){GaussianSampler::initialize();};
	GaussianSampler(World *world):UniformSampler(world){GaussianSampler::initialize();};
	GaussianSampler(RobotSim *r):UniformSampler(r){GaussianSampler::initialize();};
	Sample getNextSample();
};
/////////////////////////////////////////OLD CODE///////////////////////////////////////////



/*

class ComputeSampling2D
{
	vector<Vector2D> data;
	vector<double> dispersion;
	double maxDistance;
	Vector2D min,max;
	
public:
	void addSample(Vector2D s);
	ComputeSampling2D(Vector2D _min, Vector2D _max)
	{
		min=_min;
		max=_max;
		maxDistance=max.x-min.x;
		if(max.y-min.y>maxDistance)maxDistance=max.y-min.y;
	}
	double closestDistance(Vector2D s);
	double metric(Vector2D &a, Vector2D &b){Vector2D c(fabs(a.x-b.x),fabs(a.y-b.y));
	return c.x>c.y?c.x:c.y;
	}
	void save(char * file);
};*/





class RandomSampler2D
{
	Vector2D min,range;
public:
	RandomSampler2D(int d=2, Vector2D _min=Vector2D(0,0), Vector2D _range=Vector2D(1,1)){
		//basic range is 1 and min is 0.0
		min=_min;
		range=_range;
		srand( (unsigned)time( NULL ) );


	}
	Vector2D getNextPoint()
		{
			Vector2D p((double) rand() / (double) RAND_MAX,(double) rand() / (double) RAND_MAX) ;
			Vector2D ret(min.x+p.x*range.x,min.y+p.y*range.y);
			return ret;
		}
};
class Sampler2D
{
	//first 2D sampler is based in a predefined secquence  
protected:
	Vector2D L[4];
	int iteration;
	Vector2D min,range;
	Vector2D getNextNormalizedPoint(int n, Vector2D base, double factor);
public:
	void reset(){iteration=0;}
	Sampler2D(int d=2, Vector2D _min=Vector2D(0,0), Vector2D _range=Vector2D(1,1)){
		//basic range is 1 and min is 0.0
		iteration=0;
		initL();
		min=_min;
		range=_range;
	}
	void initL();
	virtual Vector2D getNextPoint();



};
class GaussianSampler2D: public Sampler2D
{
public:
GaussianSampler2D(int d=2, Vector2D _min=Vector2D(0,0), Vector2D _range=Vector2D(1,1));
Vector2D getNextPoint();
};

};
#endif  //__MRCORE__GENERIC_SAMPLERS__H