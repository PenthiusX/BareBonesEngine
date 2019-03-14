#include "_assetloader.h"
#include <iostream>

_AssetLoader::_AssetLoader(){
}
_AssetLoader::~_AssetLoader(){
}
std::vector<float> _AssetLoader::getAssetVertices()
{
    return this->vertices;
}

std::vector<unsigned int> _AssetLoader::getAssetIndices()
{
	return std::vector<unsigned int>();
}

bool _AssetLoader::modelLoader(QString pathToFile)
{
//	Assimp::Importer importer;

	//const aiScene* scene = importer.ReadFile(pathToFile.toStdString(),
	//	aiProcess_GenSmoothNormals |
	//	aiProcess_CalcTangentSpace |
	//	aiProcess_Triangulate |
	//	aiProcess_JoinIdenticalVertices |
	//	aiProcess_SortByPType
	//);

	//if (!scene)
	//{
	//	 std::cerr << "Error loading file: (assimp:) " << importer.GetErrorString();
	//	return false;
	//}

	return false;
}
