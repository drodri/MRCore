/**********************************************************************
 *
 * This code is part of the MRcore projec
 * Author:  Alberto Valero, Javier V. Gomez
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
 
#include "filesystem.h"
#include "base/logger.h"
#ifdef _WIN32
	#include <direct.h>

#endif

namespace mr
{
	bool createDirectory(string folder)
	{
		#ifdef _WIN32
			int ret=_mkdir(folder.c_str());
			if(ret!=0)
			{
				LOG_ERROR("Could not create directory: "<<folder);
				return false;
			}
			return true;
		#else
				return false;
		#endif
	}
	bool existDirectory(string folder)
	{
		return false;
	}

	bool isExtensionOfFile(string fileName, string extension){
		if(fileName.substr(fileName.find_last_of(".") + 1) == extension)
		{
			return true;
		}else
		{
			return false;
		}
	}
}; //end namespace
