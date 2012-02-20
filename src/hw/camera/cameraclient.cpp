#include "cameraclient.h"
#include <iostream>
#include "base/streamstring.h"

namespace mr
{
bool CameraClient::getImage(Image& image)
{
	string request(1,1);
	string response;//protocol character=1 means Ping
	int ret=sendRequest(request,response,500);
	if(ret!=COMM_OK)
	{
		return false;
	}
	else 
	{
		StreamString str(response);
		image.readFromStream(str);
		return true;
	}

	return true;
}



}; //Namespace mr
