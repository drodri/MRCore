/**********************************************************************
 *
 * This code is part of the MRcore project
 * Author:  Miguel Hernando & Diego Rodríguez-Losada & Alberto Valero
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
#include "vector3d.h"

#include <math.h>

namespace mr{

ostream& operator<<(ostream& os, const Vector3D& p)
{
	os<<p.x<<" "<<p.y<<" "<<p.z;
	return os;
}

Vector3D &Vector3D::normalize()
{
	double mod;
	mod = module();
	if(mod>(double)0.00001)
	{
		x/=mod;
		y/=mod;
		z/=mod;
	}
	return *this;
}
Vector3D Vector3D::getUnitaryVector() const
{
	Vector3D ret(*this);
	return ret.normalize();
}

//////////////////////////////////////////////////////////
Vector3D Vector3D::stringToVector3D (string cad)
{
	Vector3D p(0.00);

	if (cad.empty())
		return p;
	if (cad[0]!='{')
		return p;
	if (cad[cad.size()-1]!='}')
		return p;

	int beg=0,end=0,com=0;
	for(int i=0;i<cad.size();i++) //check the cad is correct
	{
		if (cad[i]=='{')
			beg++;
		if (cad[i]=='}')
			end++;
		if (cad[i]==',')
			com++;
	}
	if (beg!=1 || end!=1 || com!=2)
		return p;

	string aux=string();
	aux.resize(cad.size());
	int j=0,coma=0;

	for(int i=0;i<cad.size();i++)
	{
		if (cad[i]=='{')
			continue;

		else if(cad[i]==',')
		{
			if (!j)//I check there isn´t an empty vector {,,} or a wrong vector {1.0,,2.5}
				return p;//error, return the vector before overwriting
			istringstream istr(aux);
			istr>>p[coma++];
			aux.erase(0,j);
			j=0;

		}
		else if (cad[i]=='}')
		{
			istringstream istr(aux);
			istr>>p[coma++];
			return p;
		}
		else if(cad [i]==' ')
			continue;

		else
		{
			aux[j]=cad[i];
			j++;
		}

	}
	return p;

}

vector<Vector3D>  Vector3D::stringToVectorVector3D (string cad)//, string type_id)
{
	vector<Vector3D> p,aux;
	p.push_back(Vector3D(0.00));

	if (cad.empty())
		return p;
	if (cad[0]!='{')
		return p;
	if (cad[cad.size()-1]!='}')
		return p;

	int beg=0,end=0,count=0,ind=0;
	//vector<string> s;
	vector<int> size;
	for (int i=0;i<cad.size();i++)
	{
		if (cad[i]=='{')
		{
			count++;
			beg++;
		}

		if (cad[i]=='}')
		{
			end++;
			size.push_back(count);
			count=0;
		}

		if (count)
			count++;

		if(end>beg)
			return p;

	}

	if (beg!=end)
		return p;

	string s;
	int j=0,copy=0;
	ind=0;

	for (int i=0;i<cad.size();i++)
	{
		if (cad[i]=='{')
		{
			s.resize(size[ind]);
			copy=1;
			j=0;
		}
		if (cad[i]=='}')
		{
			s[j]=cad[i];
			ind++;
			copy=0;
			aux.push_back(stringToVector3D(s));
			s=string();
		}

		if(copy)
		{
			s[j]=cad[i];
			j++;
		}

	}

	return aux;
}



string Vector3D::vector3DToString (Vector3D p)
{
	stringstream str;
	string cad;
	str<<"{"<<p[0]<<","<<p[1]<<","<<p[2]<<"}";
	cad=str.str();
	return cad;
}

///////////////////////////////////////////////////////////

} //mr
