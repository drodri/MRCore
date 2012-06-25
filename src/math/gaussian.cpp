#include "gaussian.h"
#include <assert.h>
#include <iostream>
#include <exception>
using namespace std;

#include <cstdlib>
#define _USE_MATH_DEFINES
#include <math.h>

/*void gaussianTest()
{
	double med=1;
	double cov=3;
	int n=1000;
	double media=0;
	double med2=0;
	for(int i=0;i<n;i++)
	{
		double v=sampleGaussian(med,cov);
		media+=v;
		med2+=v*v;
	}
	media/=n;
	cout<<"Media: "<<media<<endl;
	cout<<"Cov: "<<med2/n-media*media<<endl;

//3D
	Vector3d realMean(1,2,3);
	vector<VectorXd> values;
	Matrix3d covar=Matrix3d::Identity()*2;
	for(int i=0;i<n;i++)
	{
		Vector3d r=sampleGaussian(realMean,covar);
		values.push_back(r);
	}
	Vector3d computedMean=mean(values);
	Matrix3d computedCov=covariance(values,computedMean);
	cout<<"Media3d: "<<computedMean<<endl;
	cout<<"Covd: "<<computedCov<<endl;
}
VectorXd mean(const vector<VectorXd>& values)
{
	//if(values.size()==0) throw exception("Empty value");
	assert(values.size()!=0);

	VectorXd m=VectorXd::Zero(values[0].size());
	for(int i=0;i<values.size();i++)
	{
		m+=values[i];
	}
	return m/(unsigned int)values.size();
}
MatrixXd covariance(const vector<VectorXd>& values,VectorXd mean)
{
	MatrixXd m=MatrixXd::Zero(mean.size(),mean.size());
	for(int i=0;i<values.size();i++)
	{
		VectorXd dif=values[i]-mean;
		m+=dif*dif.transpose();
	}
	return m/(unsigned int)values.size();
}*/
double sampleUniform(double a, double b)
{
	return a + (b-a)*rand()/(float)RAND_MAX;
}
double sampleUniform(double a)
{
	return -a + 2*a*rand()/(float)RAND_MAX;
}
double evaluateGaussian(double value, double mean, double covariance)
{
	double delta=value-mean;
	return exp(-0.5*delta*delta/covariance)/sqrt(2*M_PI*covariance);
}
double evaluateLogGaussian(double value, double mean, double covariance)
{
	double delta=value-mean;
	return -0.5*delta*delta/covariance - 0.5 *log(2*M_PI*covariance);
}
double mahalanobisDistance2(double value, double mean, double covariance)
{
	double delta=value-mean;
	return delta*delta/covariance;
}

/*double mahalanobisDistance2_n(VectorXd value, VectorXd mean, MatrixXd  info)
{
	VectorXd delta=value-mean;
	VectorXd m_dist = delta.transpose()*info*delta;
	return m_dist[0];
}

double evaluateGaussian(VectorXd value, VectorXd mean, MatrixXd cov)
{
	VectorXd dif=value-mean;
	double d=cov.determinant();
	assert(d!=0.0);

	MatrixXd inv=cov.inverse();

	VectorXd aux=inv*dif;
	double v=dif.dot(aux);
	double ret=		exp(-0.5*v)		/	sqrt(	d*	pow(2*M_PI,(int)value.size())		);

	return ret;
}*/
double sampleGaussian(double mean,double cov)
{
	double med=0;
	for(int i=0;i<12;i++)
		med+=rand()/(float)RAND_MAX;

	med-=6;//standard
	
	double x=mean+med*sqrt(cov);
	return x;
}
/*
VectorXd  sampleGaussian(VectorXd  mean,MatrixXd cov)
{
	const unsigned int n=mean.size();
	VectorXd noise(n);
	for(int i=0;i<n;i++)
		noise[i]=sampleGaussian(0,1);

	Eigen::LLT<MatrixXd> chol=cov.llt();

	VectorXd res=chol.matrixL()*noise;

	res+=mean;

	//for(int i=0;i<n;i++)
	//	res[i]+=mean[i];
	return res;

}*/

double pValue(double chi2, int dof)
{
	static double pvalues[11]={ 0.95, 0.90, 0.80, 0.70, 0.50, 0.30, 0.20, 0.10, 0.05, 0.01, 0.001	};
	static double vals[4][11]={ {0.004, 0.02, 0.06, 0.15, 0.46, 1.07, 1.64, 2.71, 3.84, 6.64 ,10.83},
								{0.10, 0.21, 0.45, 0.71, 1.39, 2.41, 3.22, 4.60, 5.99, 9.21, 13.82},
								{0.35, 0.58, 1.01, 1.42, 2.37, 3.66, 4.64, 6.25, 7.82, 11.34, 16.27 },
								{0.71, 1.06, 1.65, 2.20, 3.36, 4.88, 5.99, 7.78, 9.49, 13.28, 18.47 }	};
	assert(dof>=1 && dof<=4);

	int i=0;
	for(i=0;i<11;i++)
	{
		if(vals[dof-1][i]>chi2)
			break;
	}
	if(i>=11) i=10;
	return pvalues[i];


/*
1 0.004 0.02 0.06 0.15 0.46 1.07 1.64 2.71 3.84 6.64 10.83
2 0.10 0.21 0.45 0.71 1.39 2.41 3.22 4.60 5.99 9.21 13.82
3 0.35 0.58 1.01 1.42 2.37 3.66 4.64 6.25 7.82 11.34 16.27
4 0.71 1.06 1.65 2.20 3.36 4.88 5.99 7.78 9.49 13.28 18.47
5 1.14 1.61 2.34 3.00 4.35 6.06 7.29 9.24 11.07 15.09 20.52
6 1.63 2.20 3.07 3.83 5.35 7.23 8.56 10.64 12.59 16.81 22.46
7 2.17 2.83 3.82 4.67 6.35 8.38 9.80 12.02 14.07 18.48 24.32
8 2.73 3.49 4.59 5.53 7.34 9.52 11.03 13.36 15.51 20.09 26.12
9 3.32 4.17 5.38 6.39 8.34 10.66 12.24 14.68 16.92 21.67 27.88
10 3.94 4.86 6.18 7.27 9.34 11.78 13.44 15.99 18.31 23.21 29.59
P value (Probability) 0.95 0.90 0.80 0.70 0.50 0.30 0.20 0.10 0.05 0.01 0.001
*/

}
//Matrix sampleGaussian(Vector mean,Matrix cov)
//{

	//no es una gaussiana, sino uniforme en el 3 sigma
/*	Matrix noise(3,1);
	noise(0,0)=2.0f*(rand()/(float)RAND_MAX-0.5f);//uniform -1 to 1
	noise(0,0)*=3.0f*sqrt(cov(0,0));
	noise(1,0)=2.0f*(rand()/(float)RAND_MAX-0.5f);//uniform -1 to 1
	noise(1,0)*=3.0f*sqrt(cov(1,1));
	noise(2,0)=2.0f*(rand()/(float)RAND_MAX-0.5f);//uniform -1 to 1
	noise(2,0)*=3.0f*sqrt(cov(2,2));
	noise+=mean;
	return noise;*/


/*	const int n=mean.RowNo();

	Matrix Z(n,1);
	for(int i=0;i<n;i++)
	{
		Z(i,0)=randomGaussian();
		if(Z(i,0)>3.0f)
			Z(i,0)=3.0f;
		if(Z(i,0)<-3.0f)
			Z(i,0)=-3.0f;
	}

	Matrix A=cov.Cholesky();

	Matrix ret=mean+A*Z;

//	ret(0,0)=mean(0,0)+Z(0,0)*sqrt(cov(0,0));
//	ret(1,0)=mean(1,0)+Z(1,0)*sqrt(cov(1,1));
//	ret(2,0)=mean(2,0)+Z(2,0)*sqrt(cov(2,2));

	return ret;

}*/
/*
float randomGaussian()
{
//	float sigma=0.05f;//REFINITIVO
//	float sigma=0.2f;//COMB2
//	float sigma=0.1f;//COMB3
//	float sigma=0.075f;//COMB4 StMary
	float sigma=0.1f;//Bueno MIT
	sigma=0.4f;//prueba Indu05
//	float sigma=1.0f;
	float mu=0.0f;
	float u;
	do
	{
		u=rand()/(float)RAND_MAX;
	}
	while(fabs(u-1.0f)<0.000001);

	float z=sigma*(sqrt(2*log(1/(1-u))));
	u=rand()/(float)RAND_MAX;

	float x=mu+z*cos(2*PI*u);
	return x;
*/
	//Polar form of Box-MUller
/*	float x1, x2, w, y1, y2;
	float c = 2.3025850f;
    do
	{
            x1 = 2.0 * rand()/(float)RAND_MAX - 1.0;
            x2 = 2.0 * rand()/(float)RAND_MAX - 1.0;
            w = x1 * x1 + x2 * x2;
    } while ( w >= 1.0 );

     w = sqrt( (-2.0 * c*log( w ) ) / w );
     y1 = x1 * w;
  //   y2 = x2 * w;

	 return y1;*/
//}

static float tab[4][5]={
						{2.7055f ,  3.8415f  , 5.0239f  ,  6.6349f   , 7.8794f},
						{4.6052f ,  5.9915f  , 7.3778f  ,  9.2104f   , 10.5965f},
						{6.2514f ,  7.8147f  , 9.3484f  ,  11.3449f  , 12.8381f},
						{7.7794f ,  9.4877f  , 11.1433f ,  13.2767f  , 14.8602f}
					};


static float tab2[]={3.84f, 5.99f, 7.82f, 9.49f,  11.07f,  12.59f, 14.07f, 15.51f, 16.92f, 18.31f,
					19.68f, 21.03f, 22.36f, 23.69f, 25.00f, 26.30f, 27.59f, 28.87f, 30.14f, 31.41f,
					32.67f, 33.92f, 35.17f, 36.42f, 37.65f, 38.89f, 40.11f, 41.34f, 42.56f, 43.77f,
					44.99f, 46.19f, 47.40f, 48.60f, 49.80f, 51.00f, 52.19f, 53.38f, 54.57f, 55.76f,
					56.94f, 58.12f, 59.30f, 60.48f, 61.66f, 62.83f, 64.00f, 65.17f, 66.34f, 67.51f,
					68.67f, 69.83f, 70.99f, 72.15f, 73.31f, 74.47f, 75.62f, 76.78f, 77.93f, 79.08f,
					80.23f, 81.38f, 82.53f, 83.68f, 84.82f, 85.97f, 87.11f, 88.25f, 89.39f, 90.53f,
					91.67f, 92.81f, 93.95f, 95.08f, 96.22f, 97.35f, 98.49f, 99.62f, 100.75f,101.88f,
					103.01f, 104.14f, 105.27f, 106.40f, 107.52f, 108.65f, 109.77f, 110.90f, 112.02f, 113.15f,
					114.27f, 115.39f, 116.51f, 117.63f, 118.75f, 119.87f, 120.99f, 122.11f, 123.23f, 124.34f};


float chi2(int dof, int signif)
{
	//1=0.005 2=0.010 3=0.025 4=0.05 5=0.10
	if(signif==4)
	{
		if(dof<100&&dof>0)
			return tab2[dof-1];
		else
			return -1.0f;
	}

	if(dof<1||dof>4)
		return -1.0f;
	else
		dof-=1;

	int i=-1;
	if(signif==5)i=0;
	if(signif==4)i=1;
	if(signif==3)i=2;
	if(signif==2)i=3;
	if(signif==1)i=4;
	if(i==-1){return -1.0f;}

	return(tab[dof][i]);
}
