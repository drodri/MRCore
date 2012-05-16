/**********************************************************************
 *
 * This code is part of the MRcore project
 * Author:   Miguel Hernando Gutierrez
 * 
 *
*************************************************************************/
#include "genericsamplers.h"

Sampler::Sampler(Vector2D minim, Vector2D range){
	vector<double> m(2),r(2);
	m[0]=minim.x;m[1]=minim.y;
	r[0]=range.x;r[1]=range.y;
	configureSampler(m,r);
}
Sampler::Sampler(Vector3D minim, Vector3D range){
	vector<double> m(3),r(3);
	m[0]=minim.x;m[1]=minim.y;m[2]=minim.z;
	r[0]=range.x;r[1]=range.y;r[2]=range.z;
	configureSampler(m,r);
}
Sampler::Sampler(BoundingBox &box){
	vector<double> m(3),r(3);
	Vector3D minim=box.getMinVertex();
	Vector3D range=box.getMaxVertex()-minim;
	m[0]=minim.x;m[1]=minim.y;m[2]=minim.z;
	r[0]=range.x;r[1]=range.y;r[2]=range.z;
	configureSampler(m,r);
}
void Sampler::setWorld(World *world)
{
	vector<double> m(3),r(3);
	BoundingBox box=world->getBoundingBox();
	//hago lo mismo que en el constructor anterior
	Vector3D minim=box.getMinVertex();
	Vector3D range=box.getMaxVertex()-minim;
	m[0]=minim.x;m[1]=minim.y;m[2]=minim.z;
	r[0]=range.x;r[1]=range.y;r[2]=range.z;
	configureSampler(m,r);
}
Sampler::Sampler(World *world){
	setWorld(world);
}
Sampler::Sampler(RobotSim *robot){
	//grados de libertad y max y min de cada articulacion
	int n=robot->getNumJoints();
	vector<double> m(n),r(n);
	double val;
	for(int i=0;i<n;i++){robot->getJointLimits(i,val,m[i]);r[i]=val-m[i];}
	configureSampler(m,r);
}
void Sampler::configureSampler( vector<double> _min, vector<double> _range ){
		if(_min.size()!=_range.size())return;
		dimension=(int)_min.size();
		min=_min; range=_range; sample=min;
		normalizedSample=new double[dimension];
	}
const Sample &Sampler::computeSample(){
		for(int i=0;i<dimension;i++)sample[i]=min[i]+normalizedSample[i]*range[i];
		return sample;
	}
////RANDOM SAMPLER: example of how to construct a simple n dimensional sampler 
Sample RandomSampler::getNextSample()
{
	for(int i=0;i<getDimension();i++)
		normalizedSample[i]=(double) rand() / (double) RAND_MAX;
	return computeSample();
}
double Sampler::sampleGaussian(double mean,double cov)
{
	double med=0;
	for(int i=0;i<12;i++)
		med+=rand()/(float)RAND_MAX;

	med-=6;//standard
	double x=mean+med*sqrt(cov)/6;
	return x;
}
double Sampler::sampleGaussian(int n,double mean,double cov)
{
	double med=0;
	for(int i=0;i<n;i++)
		med+=rand()/(float)RAND_MAX;

	med-=(n/2);//standard
	double x=mean+2*med*sqrt(cov)/n;
	return x;
}
//UNIFORMSAMPLER 
void UniformSampler::initializeL()
{
	int n=getDimension(),i,j;

	iteration=0;
	if(n>8*sizeof(int)-1)
		return; //MIKE, this exception is not supported in g++4.4 throw exception("Uniform Sampler: dimension too high");
	L_n=1<<n;
	L=new double *[L_n];
	for(int i=0;i<L_n;i++)L[i]=new double[n];


//computation is based on the aTd Matrix of Rossell (2007 Int Journal Robot Systems)
	//T is an N*N  char matrix (0,1)
	typedef unsigned char bit;
	bit **T=new bit *[n];
	bit *aux=new bit[n];
	for(i=0;i<n;i++){T[i]=new bit[n];}
	//la construyo por columnas
	for(j=0;j<n;j++)
		for(i=0;i<n;i++)
		{
			if(i==j)T[i][j]=1;
			if(i<j)T[i][j]=0;
			if((i>j)&&(j!=0)){
				int k=(i-j-1)/(j);
				
				if(k%2==0)T[i][j]=0;
				else T[i][j]=1;
			}
			if(j==0)T[i][j]=1;
		}

	//obtengo los valores de L
		for(i=0;i<L_n;i++){
			//representacion bit a bit
			for(j=0;j<n;j++)if(i&(0x1<<j))aux[j]=1;else aux[j]=0;
			
			for(int k=0;k<n;k++){
				int val=0;
				
				if(n==2)val=L_2[i][k];
				else if(n==3)val=L_3[i][k];
				else if(n==6)val=L_6[i][k];
				else for(int l=0;l<n;l++)val+=T[k][l]*aux[l];

				L[i][k]=0.5*(val%2);
			}

		}
	//al terminar hay que eliminar T y aux
		for(i=0;i<n;i++)delete [] T[i];
		delete [] aux;
}
//RECURSIVE METHOD - 2003 IEEE ICRA Lindermann
Sample UniformSampler::getNextNormalizedSample(int n, Sample base, double factor)
{
	int i;
	Sample sample=base;
	int index=n%L_n;
	int nextN=n/L_n;
	for(i=0;i<getDimension();i++)sample[i]=sample[i]+L[index][i]*factor;
	if(nextN==0) //return sample+Vector2D(factor/4,factor/4);
	{
		for(i=0;i<getDimension();i++)sample[i]=sample[i]+factor/4; //Shukarev Correction
		return sample;
	}
	else return getNextNormalizedSample(nextN,sample,factor/2.0);
}

UniformSampler::~UniformSampler(){
	if(L==0)return;
	for(int i=0;i<L_n;i++)delete [] L[i];
}
Sample UniformSampler::getNextSample()
{
	Sample seed(getDimension());
	for(int i=0;i<getDimension();i++)seed[i]=0.0;
	Sample aux=getNextNormalizedSample(iteration, seed, 1.0);
	iteration++;
	for(int j=0;j<getDimension();j++)
		normalizedSample[j]=aux[j];
	return computeSample();
}
//static values
	double UniformSampler::L_2[4][2]={{0,0},{1,1},{1,0},{0,1}};
	double UniformSampler::L_3[8][3]={{0,0,0},{1,1,1},{0,1,0},{1,0,1},{0,0,1},{1,1,0},{0,1,1},{1,0,0}};
	double UniformSampler::L_6[64][6]={
		{0,0,0,0,0,0}, {1,1,1,1,1,1}, {0,1,0,1,1,1}, {1,0,1,0,0,0}, {0,0,1,1,1,1}, {1,1,0,0,0,0}, {0,1,1,0,0,0}, {1,0,0,1,1,1}, 
		{0,0,0,1,0,1}, {1,1,1,0,1,0}, {0,1,0,0,1,0}, {1,0,1,1,0,1}, {0,0,1,0,1,0}, {1,1,0,1,0,1}, {0,1,1,1,0,1}, {1,0,0,0,1,0},
		{0,0,0,0,1,1}, {1,1,1,1,0,0}, {0,1,0,1,0,0}, {1,0,1,0,1,1}, {0,0,1,1,0,0}, {1,1,0,0,1,1}, {0,1,1,0,1,1}, {1,0,0,1,0,0}, 
		{0,0,0,1,1,0}, {1,1,1,0,0,1}, {0,1,0,0,0,1}, {1,0,1,1,1,0}, {0,0,1,0,0,1}, {1,1,0,1,1,0}, {0,1,1,1,1,0}, {1,0,0,0,0,1},
		{0,0,0,0,0,1}, {1,1,1,1,1,0}, {0,1,0,1,1,0}, {1,0,1,0,0,1}, {0,0,1,1,1,0}, {1,1,0,0,0,1}, {0,1,1,0,0,1}, {1,0,0,1,1,0},
		{0,0,0,1,0,0}, {1,1,1,0,1,1}, {0,1,0,0,1,1}, {1,0,1,1,0,0}, {0,0,1,0,1,1}, {1,1,0,1,0,0}, {0,1,1,1,0,0}, {1,0,0,0,1,1},
		{0,0,0,0,1,0}, {1,1,1,1,0,1}, {0,1,0,1,0,1}, {1,0,1,0,1,0}, {0,0,1,1,0,1}, {1,1,0,0,1,0}, {0,1,1,0,1,0}, {1,0,0,1,0,1}, 
		{0,0,0,1,1,1}, {1,1,1,0,0,0}, {0,1,0,0,0,0}, {1,0,1,1,1,1}, {0,0,1,0,0,0}, {1,1,0,1,1,1}, {0,1,1,1,1,1}, {1,0,0,0,0,0}};

//////GAUSSIAN SAMPLER
Sample GaussianSampler::getNextSample()
{
	int number=iteration;
	double cellsize=0.5F, cov;
	while(number>0){
		number/=L_n;
		cellsize/=2;
	}
	if(iteration==0)cellsize=0.25;

	cov=cellsize*cellsize;
	UniformSampler::getNextSample();
	for(int j=0;j<getDimension();j++)
		normalizedSample[j]=sampleGaussian(6,normalizedSample[j],cov);
	return computeSample();
}