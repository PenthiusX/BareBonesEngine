#include "_assetloader.h"
#include <iostream>
#include <tools.h>
#include <string>
//
//GLM math Library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//
#include <string>
#include <stdlib.h>
#include <sstream>

_AssetLoader::_AssetLoader()
{

}
_AssetLoader::~_AssetLoader()
{

}
/*
*/
std::vector<float> _AssetLoader::getAssetVertices()
{
    return this->vertices;
}
/*
*/
std::vector<unsigned int> _AssetLoader::getAssetIndices()
{
	return this->indices;
}
/*
* Custom Obj Loader
*/
bool _AssetLoader::objLoader(QString pathToFile)
{
	QByteArray qba = ReadStringFromQrc(pathToFile).toLocal8Bit(); // get shader source from qrc file
	const char* p = qba; //convert to const char*

	std::string objData = p;
	std::string temp = "0000000";
	std::string temp2 = "0000000";

	for(int i = 0 ; i < objData.size(); i++)
	{
		if (objData[i] == 'v' && objData[i+1] == ' ')
		{
			for(int j = 0; j < 28; j++)
			{
				if(objData[i + j] == ' ')
				{
					for (int k = 0; k < 8; k++)
					{
						temp[k] = objData[i + j + k];
					}
					vertices.push_back(atof(temp.data()));
				}
			}
		}
	}


	std::string a = objData.substr(objData.find("f ") + 1);
	std::stringstream ss;
	/* Storing the whole string into string stream */
	ss << a;

	/* Running loop till the end of the stream */
	int found;
	while (!ss.eof()) 
	{

		/* extracting word by word from stream */
		ss >> temp2;

		/* Checking the given word is integer or not */
		if (std::stringstream(temp2) >> found)
		{
			indices.push_back(found - 1);
		}
		/* To save from space at the end of string */
		temp2 = "";
	}
	return false;
}