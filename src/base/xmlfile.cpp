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
#include "xmlfile.h"
#include "object.h"

namespace mr
{

XMLfile::XMLfile (const char *file)
{
	xml = new XML(file);
	root = xml->GetRootElement();

	checkOverwrite=false;

	if (root->GetChildrenNum())
		checkOverwrite=true;

}

XMLfile::XMLfile (XMLElement *parent)
{
	root = parent;
}

XMLfile::~XMLfile()
{
//	delete xml;
}


XMLfile& XMLfile::operator<<(Object *obj)
{
	write(obj);
	return (*this);
}

XMLfile& XMLfile::operator<<(int _id)
{
//	XMLAux aux;
	XMLVariable* ident= new XMLVariable ("id",XMLAux::string_Convert<int>(_id).c_str());
	pElem[_id]->AddVariable(ident);
	return (*this);

}



void XMLfile::write (Object* pObj)
{
	if (checkOverwrite)//if true means than the file.xml which already exists, is going to be overwritten
	{
		root->RemoveAllElements();//remove old xml elements
		checkOverwrite=false;
	}

	string className = pObj->getClassName();
	XMLElement* obj_xml=new XMLElement (root,className.c_str());
	
	pObj->writeToXML(obj_xml);

	root->AddElement(obj_xml);
	pElem.push_back(obj_xml);//save every XMLElements_Objects in pElem

}

Object* XMLfile::read (XMLElement* obj)
{
	char className[50]={0};
	obj->GetElementName(className);
	Object*	pObj=Object::create(string(className));
	if(pObj)
	{
		pObj->readFromXML(obj);
		return pObj;
	}
	else
		return 0;
}

bool XMLfile::save()
{
	xml->Save();
	return true;
}


bool XMLfile::save(Object* pObj,const char *file) //saves the XML file created with the object
{
		// Create the XML FILE or if it already exists, it will be overwritten

	XML* xml = new XML(file);
	XMLElement* root = xml->GetRootElement();

	string className = pObj->getClassName();
	XMLElement* obj_xml=new XMLElement (root,className.c_str());

	pObj->writeToXML(obj_xml);//execute the writing process XML
	pElem.push_back(obj_xml);//save every XMLElements_Objects in pElem

	root->AddElement(obj_xml);
	xml->Save();

	return true;

}


Object* XMLfile::load(const char *file) //loads, parses the file and creates the object
{

	//import another objects saved in another files xml
	if (importFromXML())
	{
		if (root->FindElementZ("World"))
			root->FindElementZ("World")->AddElement(imported);
		else
			root->AddElement(imported);
	}

	XMLElement** objects=root->GetChildren();
	int num=root->GetChildrenNum();

	char className[50]={0};
	for (int i=0; i<num; i++)
	{
		if (objects[i]!=root->FindElementZ("importfrom"))
		{
			objects[i]->GetElementName(className);
			Object*	pObj=Object::create(string(className));
			if(pObj)
			{
				pObj->readFromXML(objects[i]);
				return pObj;
			}
		}
	}

	return 0;
}


bool XMLfile::importFromXML ()
{

	//Ejemplo: <importfrom xml="eutibot.xml" element="euitibot" name="Robot1">
	string name;

	if (root->FindElementZ("importfrom"))
	{
		XMLElement* import=root->FindElementZ("importfrom");
		if (import->FindVariableZ("xml"))
		{
			name=XMLAux::GetValueCadena(import->FindVariableZ("xml"));
			XML* xmlImport = new XML(name.c_str());
			XMLElement* rootImport = xmlImport->GetRootElement();


			if (import->FindVariableZ("element"))
			{
				name=string();
				name=XMLAux::GetValueCadena(import->FindVariableZ("element"));

				if (rootImport->FindElementZ(name.c_str()))
				{
					imported=rootImport->FindElementZ(name.c_str())->Duplicate();
					if (import->FindVariableZ("name"))
					{
						name=string();
						name=XMLAux::GetValueCadena(import->FindVariableZ("name"));

						if (imported->FindVariableZ("name"))
							imported->FindVariableZ("name")->SetValue(name.c_str());
						else
						{
							XMLVariable* _name=new XMLVariable ("name",name.c_str());
							imported->AddVariable(_name);
						}
					}
					return true;
				}

				else //search the XMLElement in another deep just in case
				{
					int num=rootImport->GetAllChildrenNum();
					XMLElement** childsImport=new XMLElement*[num];
					rootImport->GetAllChildren(childsImport);
					for (int i=0;i<num;i++)
					{
						if (childsImport[i]->FindElementZ(name.c_str()))
						{
							imported=childsImport[i]->FindElementZ(name.c_str())->Duplicate();

							if (import->FindVariableZ("name"))
							{
								name=string();
								name=XMLAux::GetValueCadena(import->FindVariableZ("name"));

								if (imported->FindVariableZ("name"))
									imported->FindVariableZ("name")->SetValue(name.c_str());
								else
								{
									XMLVariable* _name=new XMLVariable ("name",name.c_str());
									imported->AddVariable(_name);
								}
							}
							return true;
						}
					}


				}


			}

		}


	}

	return false;
}

/*overwrite()
{



}
*/
} ;//mr
