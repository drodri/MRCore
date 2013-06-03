/**********************************************************************
 *
 * This code is part of the MRcore project
 * Authors:  Miguel Hernando, Diego Rodríguez-Losada, Paloma de la Puente,
 *			 Alberto Valero, Luis Pedraza
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
#ifndef __MRCORE__MATRIX3x3_H
#define __MRCORE__MATRIX3x3_H

#include <iostream>


#include "vector3d.h"

namespace mr
{
/*!
    \class Matrix3x3
    \brief Matrix3x3 - a basic mathematical class for 3x3 matrix.
	It is any 3x3 matrix not only an orientation matrix, therefore, 
	operations like inversion are made through a general formula. Another
	consequence is that the matrix elements are public.
*/
class Matrix3x3{
	friend class Vector3D;

//attributes.
protected:
	union{
		
		double mat[3][3];
		struct{
			double dat[3][3];
		}matrix;
	};
public:
//constructors
	inline Matrix3x3(double diag=1); 
	inline Matrix3x3(double m00, double m01, double m02,
					double m10, double m11, double m12,
					double m20, double m21, double m22);
	
	
	
//operators
	inline double * operator [](int i){return mat[i];}
	inline Matrix3x3 operator * (const Matrix3x3 &m)const;
	inline Matrix3x3 & operator *= (const Matrix3x3 &m);
	inline Matrix3x3 operator+ (const Matrix3x3 &m )const;
	inline Matrix3x3 & operator+= (const Matrix3x3 &m );
	inline Matrix3x3 operator- (const Matrix3x3 &m )const;
	inline Matrix3x3 & operator-= (const Matrix3x3 &m );
	inline Matrix3x3 & operator*= (double v);
	inline Matrix3x3 & operator/= (double v);
	inline Vector3D operator * (const Vector3D &v) const;
	inline bool operator== (const Matrix3x3 &m );
//methods
	inline double getDeterminant() const;
	inline Matrix3x3 inverted() const;
	inline Matrix3x3 transposed() const;
//friend operators
	friend std::ostream &operator << (std::ostream &s, const Matrix3x3 &v);
	
	friend inline Matrix3x3 operator* (double scalar, const Matrix3x3 & m);
	friend inline Matrix3x3 operator/ (double scalar, const Matrix3x3 & m);


//friend methods
};

//Implementation of inline methods/////////////////////////////////////

bool  Matrix3x3::operator== (const Matrix3x3 &m )
{
	if (mat[0][0]==m.mat[0][0] && mat[0][1]==m.mat[0][1] && mat[0][2]==m.mat[0][2] &&
	mat[1][0]==m.mat[1][0] && mat[1][1]==m.mat[1][1] && mat[1][2]==m.mat[1][2] &&
	mat[2][0]==m.mat[2][0] && mat[2][1]==m.mat[2][1] && mat[2][2]==m.mat[2][2])
		return true;
	else
		return false;

}

Matrix3x3::Matrix3x3(double diag)
{
	mat[0][0]=mat[1][1]=mat[2][2]=diag;
	mat[0][1]=mat[0][2]=mat[1][0]=mat[1][2]=mat[2][0]=mat[2][1]=0;
}

Matrix3x3::Matrix3x3(double m00, double m01, double m02,
               double m10, double m11, double m12,
			   double m20, double m21, double m22)
{
	mat[0][0]=m00;mat[0][1]=m01;mat[0][2]=m02;
	mat[1][0]=m10;mat[1][1]=m11;mat[1][2]=m12;
	mat[2][0]=m20;mat[2][1]=m21;mat[2][2]=m22;
}


double Matrix3x3::getDeterminant()const
{
  return (-mat[0][2] * mat[1][1] * mat[2][0] + 
		 mat[0][1] * mat[1][2] * mat[2][0] + 
		 mat[0][2] * mat[1][0] * mat[2][1] - 
		 mat[0][0] * mat[1][2] * mat[2][1] - 
		 mat[0][1] * mat[1][0] * mat[2][2] + 
		 mat[0][0] * mat[1][1] * mat[2][2] );
}
Matrix3x3 Matrix3x3::inverted() const
{
	double val=getDeterminant();
	Matrix3x3 aux(*this);
	if(val!=0){ //FIX_ME  INCLUDE EXCEPTIONS
		val=1/val;
		aux.mat[0][0]=val*(-mat[1][2] * mat[2][1] + mat[1][1] * mat[2][2]);
		aux.mat[0][1]=val*(mat[0][2] * mat[2][1] - mat[0][1] * mat[2][2]);
		aux.mat[0][2]=val*(-mat[0][2] * mat[1][1] + mat[0][1] * mat[1][2]);
		aux.mat[1][0]=val*(mat[1][2] * mat[2][0] - mat[1][0] * mat[2][2]);
		aux.mat[1][1]=val*(-mat[0][2] * mat[2][0] + mat[0][0] * mat[2][2]);
		aux.mat[1][2]=val*(mat[0][2] * mat[1][0] - mat[0][0] * mat[1][2]);
		aux.mat[2][0]=val*(-mat[1][1] * mat[2][0] + mat[1][0] * mat[2][1]);
		aux.mat[2][1]=val*(mat[0][1] * mat[2][0] - mat[0][0] * mat[2][1]);
		aux.mat[2][2]=val*(-mat[0][1] * mat[1][0] + mat[0][0] * mat[1][1]);
		}
	return aux;
}
Matrix3x3 Matrix3x3::transposed() const
{
	return Matrix3x3( mat[0][0], mat[1][0], mat[2][0],
				 mat[0][1], mat[1][1], mat[2][1],
                 mat[0][2], mat[1][2], mat[2][2] );
	
}

Matrix3x3 Matrix3x3::operator * (const Matrix3x3 &t) const
{
	return Matrix3x3(
		(mat[0][0])*(t.mat[0][0])+(mat[0][1])*(t.mat[1][0])+(mat[0][2])*(t.mat[2][0]),
		(mat[0][0])*(t.mat[0][1])+(mat[0][1])*(t.mat[1][1])+(mat[0][2])*(t.mat[2][1]),
		(mat[0][0])*(t.mat[0][2])+(mat[0][1])*(t.mat[1][2])+(mat[0][2])*(t.mat[2][2]),
		(mat[1][0])*(t.mat[0][0])+(mat[1][1])*(t.mat[1][0])+(mat[1][2])*(t.mat[2][0]),
		(mat[1][0])*(t.mat[0][1])+(mat[1][1])*(t.mat[1][1])+(mat[1][2])*(t.mat[2][1]),
		(mat[1][0])*(t.mat[0][2])+(mat[1][1])*(t.mat[1][2])+(mat[1][2])*(t.mat[2][2]),
		(mat[2][0])*(t.mat[0][0])+(mat[2][1])*(t.mat[1][0])+(mat[2][2])*(t.mat[2][0]),
		(mat[2][0])*(t.mat[0][1])+(mat[2][1])*(t.mat[1][1])+(mat[2][2])*(t.mat[2][1]),
		(mat[2][0])*(t.mat[0][2])+(mat[2][1])*(t.mat[1][2])+(mat[2][2])*(t.mat[2][2]));
}
Matrix3x3 & Matrix3x3::operator *= (const Matrix3x3 &t)
{
	(*this)=(*this)*t;
	return *this;
}
Matrix3x3 Matrix3x3::operator+ (const Matrix3x3 &m ) const
{
	return Matrix3x3( 
		mat[0][0]+m.mat[0][0],mat[0][1]+m.mat[0][1],mat[0][2]+m.mat[0][2],
		mat[1][0]+m.mat[1][0],mat[1][1]+m.mat[1][1],mat[1][2]+m.mat[1][2],
		mat[2][0]+m.mat[2][0],mat[2][1]+m.mat[2][1],mat[2][2]+m.mat[2][2]);
}


Matrix3x3 & Matrix3x3::operator+= (const Matrix3x3 &m )
{
		mat[0][0]+=m.mat[0][0];mat[0][1]+=m.mat[0][1];mat[0][2]+=m.mat[0][2];
		mat[1][0]+=m.mat[1][0];mat[1][1]+=m.mat[1][1];mat[1][2]+=m.mat[1][2];
		mat[2][0]+=m.mat[2][0];mat[2][1]+=m.mat[2][1];mat[2][2]+=m.mat[2][2];
	return (*this);
}
Matrix3x3 Matrix3x3::operator- (const Matrix3x3 &m ) const
{
	return Matrix3x3( 
		mat[0][0]-m.mat[0][0],mat[0][1]-m.mat[0][1],mat[0][2]-m.mat[0][2],
		mat[1][0]-m.mat[1][0],mat[1][1]-m.mat[1][1],mat[1][2]-m.mat[1][2],
		mat[2][0]-m.mat[2][0],mat[2][1]-m.mat[2][1],mat[2][2]-m.mat[2][2]);
}
Matrix3x3 & Matrix3x3::operator-= (const Matrix3x3 &m )
{
		mat[0][0]-=m.mat[0][0];mat[0][1]-=m.mat[0][1];mat[0][2]-=m.mat[0][2];
		mat[1][0]-=m.mat[1][0];mat[1][1]-=m.mat[1][1];mat[1][2]-=m.mat[1][2];
		mat[2][0]-=m.mat[2][0];mat[2][1]-=m.mat[2][1];mat[2][2]-=m.mat[2][2];
	return (*this);
}
Matrix3x3 & Matrix3x3::operator*= (double v)
{
		mat[0][0]*=v;mat[0][1]*=v;mat[0][2]*=v;
		mat[1][0]*=v;mat[1][1]*=v;mat[1][2]*=v;
		mat[2][0]*=v;mat[2][1]*=v;mat[2][2]*=v;
	return (*this);
}
Matrix3x3 & Matrix3x3::operator/= (double v)
{
		mat[0][0]/=v;mat[0][1]/=v;mat[0][2]/=v;
		mat[1][0]/=v;mat[1][1]/=v;mat[1][2]/=v;
		mat[2][0]/=v;mat[2][1]/=v;mat[2][2]/=v;
	return (*this);
}
Matrix3x3 operator* (double v, const Matrix3x3 & m)
{
	return (Matrix3x3(m)*=v);
}
Matrix3x3 operator/ (double v, const Matrix3x3 & m)
{
	return (Matrix3x3(m)/=v);
}
Vector3D Matrix3x3::operator * (const Vector3D &v) const
{
	return Vector3D( v*(mat[0]),v*(mat[1]),v*(mat[2]));
}
}
#endif  //__MRCORE__MATRIX3x3_H
