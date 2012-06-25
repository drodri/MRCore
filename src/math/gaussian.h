#pragma once


#define NOMINMAX
#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

//#include "../../math/eigen/Eigen/Core"
//#include "../../math/eigen/Eigen/Cholesky"
//#include "../../math/eigen/Eigen/Dense"

//USING_PART_OF_NAMESPACE_EIGEN

//using namespace Eigen;

//typedef DVector<double> Vector;
//typedef DMatrix<double> Matrix;

double evaluateGaussian(double value, double mean, double covariance);
double evaluateLogGaussian(double value, double mean, double covariance);

///return square of mahalanobis distance
double mahalanobisDistance2(double value, double mean, double covariance);
//double mahalanobisDistance2_n(VectorXd  value, VectorXd mean, MatrixXd  info);
double pValue(double chi2, int dof);

//double evaluateGaussian(VectorXd value, VectorXd mean, MatrixXd cov);
double sampleUniform(double a, double b);//interval [a b]
double sampleUniform(double a); //Interval [-a a]

double sampleGaussian(double mean,double cov);
//VectorXd sampleGaussian(VectorXd mean,MatrixXd cov);
//VectorXd mean(const vector<VectorXd>& values);
//MatrixXd covariance(const vector<VectorXd>& values,VectorXd mean);
void gaussianTest();
