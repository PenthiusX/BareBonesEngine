#include "_assetloader.h"
#include <iostream>
#include <tools.h>
#include <string>
#include <stdlib.h>
#include <sstream>
//GLM math Library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


/*
* Constructor/Distructor:
* Created:15_03_2019
*/
_AssetLoader::_AssetLoader()
{

}
_AssetLoader::~_AssetLoader()
{

}
/*
* Function:getAssetVertices()
* returns the vertices array for the model obeject
* form the model object loader
* Created:15_03_2019
*/
std::vector<float> _AssetLoader::getAssetVertices()
{
    return this->vertices;
}
/*
* Function:getAssetIndices()
* returns the indices array which gets 
* filled in the objloader.
* Created:15_03_2019
*/
std::vector<unsigned int> _AssetLoader::getAssetIndices()
{
	return this->indices;
}
/*
* Function:objLoader(QString pathToFile)
* takes a Qstring path to qrc file and then parses through 
* a string array to get the approptie onfo to render the model
* Created:15_03_2019
*/
bool _AssetLoader::objLoader(QString pathToFile)
{
	QByteArray qba = ReadStringFromQrc(pathToFile).toLocal8Bit();
	const char* p = qba;

	std::string objData = p;
	std::string temp = "0000000";
	std::string temp2 = "0000000";

	std::string av = objData.substr(objData.find("v ") + 1);
	std::stringstream ssv;
	ssv << av;
	float foundf;
	while (!ssv.eof())
	{
		ssv >> temp;
		if (std::stringstream(temp) >> foundf)
		{
			vertices.push_back(foundf - 1);
		}
		temp = "";
	}

	std::string ai = objData.substr(objData.find("f ") + 1);
	std::stringstream ss;
	ss << ai;
	int foundi;
	while (!ss.eof()) 
	{
		ss >> temp2;
		if (std::stringstream(temp2) >> foundi)
		{
			indices.push_back(foundi - 1);
		}
		temp2 = "";
	}
	return false;
}
