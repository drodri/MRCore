/**********************************************************************
 *
 * This code is part of the MRcore project
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
#include "xmlaux.h"


namespace mr
{

string XMLAux::string_ConvertBool (bool b)
	{
		string cad;
		if (b) cad="true";
		else cad="false";
		return cad;
	}

double XMLAux::GetValueDouble(XMLVariable* v)
	{
		if (!v)
			return 0.00;
		char cad[50]={0};
		v->GetValue(cad);
		istringstream istr(cad);
		double p=0.000;
		istr>>p;
		return p;
	}
string XMLAux::GetValueCadena (XMLVariable* v)
{
	if (!v)
		return "noName";
	char t[50] = {0};
	v->GetValue(t);
	string cad=string(t);
	return cad;
}
string XMLAux::GetNameElement (XMLElement *elem)
{
	if (!elem)
		return "error";
	char t[50] = {0};
	elem->GetElementName(t);
	string cad=string(t);
	return cad;
}

string XMLAux::setLinkToTcpDefault (string name_owner,int index_tcp,int id_owner)
{
	stringstream str;
	if (name_owner=="noName")
		str<<id_owner<<'.'<<index_tcp;
	else
		str<<'$'<<name_owner<<'$'<<".tcp["<<index_tcp<<']';
	string cad=str.str();
	return cad;
}
string XMLAux::setLinkToTcp (string name_owner,string name_tcp,int index_tcp,int id_owner)
{
	stringstream str;
	if (name_owner=="noName")
		str<<id_owner<<'.'<<index_tcp;
	else
		str<<'$'<<name_owner<<'$'<<'.'<<name_tcp<<'['<<index_tcp<<']';
	string cad=str.str();
	return cad;
}
string XMLAux::setLinkTo (string name_link,int id_link)
{
	stringstream str;
	if (name_link=="noName")
		str<<id_link;
	else
		str<<'$'<<name_link<<'$';
	string cad=str.str();
	return cad;
}

//XMLElement* XMLAux::getXMLObject (XMLElement* parent, Object *obj,int index_obj)
//{
//	int num=parent->GetChildrenNum();
//	string className = obj->getClassName();
//	if (num)
//	{
//		XMLElement** childs=parent->GetChildren();
//		for (int i=0;i<num;i++)
//		{
//			if(GetNameElement(childs[i])==className && childs[i]->FindVariableZ("id")->GetValueInt()==index_obj)
//				return childs[i];
//		}
//	}
//	return 0;
//}

void XMLAux::GetValueOwnerAndTcp(XMLVariable* v, int* ownerAndTcp)
	{
		char cad[50]={0};
		v->GetValue(cad);
		string check=string(cad);
		stringstream str,str1;
		bool toTcp=false;
		for (int i=0;i<(int)check.size();i++)
		{
			if (check[i]==',' || check[i]=='.')
			{
				toTcp=true;
				i++;
			}
			if (!toTcp)
				str<<check[i];
			else
				str1<<check[i];

		}

		str>>(int)ownerAndTcp[0];
		str1>>(int)ownerAndTcp[1];

	}
string XMLAux::GetNameLinkTo (XMLVariable* v)
{
	char cad[50]={0};
	v->GetValue(cad);
	string check=string(cad);
	int ident=0;
	stringstream str;

	for (int i=0;i<(int)check.size();i++)
	{
		if (check[i]=='$')
		{
			i++;
			ident++;
		}
		if(ident<2)
			str<<check[i];
	}
	string link=str.str();
	return link;
}

vector<string> XMLAux::GetNameOwnerAndTcp(XMLVariable* v)
{
	vector<string> ownerAndTcp;
	string owner=string();
	string tcp=string();
	ownerAndTcp.push_back(owner);
	ownerAndTcp.push_back(tcp);

	char cad[50]={0};
	v->GetValue(cad);
	string check=string(cad);

	bool toTcp=false;
	int j=0;
	stringstream str1,str2;

	for (int i=0;i<(int)check.size();i++)
	{
		if (check[i]=='$')
			i++;

		if (check[i]==',' || check[i]=='.')
		{
			toTcp=true;
			j=0;
			i++;
		}
		if (!toTcp)
		{
			str1<<check[i];
			j++;
		}
		else
		{
			str2<<check[i];
			j++;
		}

	}
	ownerAndTcp[0]=str1.str();
	ownerAndTcp[1]=str2.str();
	return ownerAndTcp;
}

int XMLAux::getIndTcp (string cad)
{
	int parentBeg=0,parentEnd=0;
	bool index=false,indDefault=true;
	stringstream str;
	int ind=-1;
	for (int i=0;i<(int)cad.size();i++)
	{
		if (cad[i]=='[' || cad[i]=='(')
		{
			parentBeg++;
			i++;
			index=true;
		}
		if (cad[i]==']'|| cad[i]==')')
		{
			parentEnd++;
			index=false;
		}
		if (index)
		{
			indDefault=false;
			str<<cad[i];
		}

	}
	if (parentBeg!=parentEnd)
		return -1;
	if (indDefault)
		return 0;

	str>>(int)ind;
	return ind;
}


string  XMLAux::getTypeConectionLink (XMLVariable* v,bool linktotcp)
{
		char t[50] = {0};
		v->GetValue(t);
		string cad=string(t);
		int ident=0,separate=0;

		for (int i=0;i<(int)cad.size();i++)
		{
			if (cad[i]=='$')
				ident++;
			if (cad[i]==',' || cad[i]=='.')
				separate++;
		}

		if (ident==2 && separate==1 && linktotcp)
			return "conectionNames";

		else if (ident==0 && separate==1 && linktotcp)
			return "conectionId";

		else if (ident==2 && !separate && !linktotcp)
			return "conectionNames";

		else if (ident==0 && !separate && !linktotcp)
			return "conectionId";

		else
			return "error";

}

string XMLAux::getOnlyNameTcp(string name)
{
	stringstream str;

	for (int i=0;i<(int)name.size();i++)
	{
		if (name[i]=='[')
			break;
		str<<name[i];

	}
	string onlyname=str.str();
	//check only name is correct
	for (int i=0;i<(int)onlyname.size();i++)
	{
		if (onlyname[i]=='[' || onlyname[i]=='(' || onlyname[i]==']'|| onlyname[i]==')')
			return "Positionable Entity";
	}
	return onlyname;
}


bool XMLAux::matrixTypeEuler (string cad)
{
	int beg=0,end=0;
	for(int i=0;i<cad.size();i++) //checking the cad
	{
		if (cad[i]=='{')
			beg++;
		if (cad[i]=='}')
			end++;

	}
	if (beg==3 && end==3)//it's the only possibility
		return false;
	else
		return true;//another ones are type Euler
	//if it's written wrong, the function in vector3D return the vector (0,0,0)
}




} ;//mr
