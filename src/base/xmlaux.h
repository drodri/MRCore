/**********************************************************************
 *
 * This code is part of the MRcore projec
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
#ifndef __MRCORE__XMLAUX__H
#define __MRCORE__XMLAUX__H


#include <string>
#include <sstream>
#include <iostream>
#include "xml.h"
#include <vector>
//#include "object.h"

using namespace std;


namespace mr
{


 //XML &operator<<(XML &xml, const Object &obj);


class XMLAux
{
public:

XMLAux(void){};

template <class T> static string string_Convert (T x)
{
	stringstream str;
	string cad;
	str<<x;
	cad=str.str();
	//str.str(string()); clean str
	return cad;
}

static	string string_ConvertBool (bool b);
static double GetValueDouble(XMLVariable* v);
static string GetValueCadena (XMLVariable* v);
static string GetNameElement (XMLElement *elem);
static string setLinkTo (string name_link,int id_link);
static string GetNameLinkTo (XMLVariable* v);

//static XMLElement* getXMLObject (XMLElement* parent, Object *obj,int index_obj);



//help functions to Tcp links
static void GetValueOwnerAndTcp(XMLVariable* v, int* ownerAndTcp);
static vector<string> GetNameOwnerAndTcp(XMLVariable* v);
static string getTypeConectionLink (XMLVariable* v, bool linktotcp=true);
static int getIndTcp (string cad);
static string setLinkToTcpDefault (string name_owner,int index_tcp,int id_owner);
static string setLinkToTcp (string name_owner,string name_tcp,int index_tcp,int id_owner);
static string getOnlyNameTcp(string name);

};



};
#endif  //__MRCORE__XMLAUX__H
