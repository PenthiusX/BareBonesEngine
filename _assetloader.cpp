#include "_assetloader.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <sstream>

#include <qdiriterator.h>
#include <QXmlStreamReader>

#include "_tools.h"

/*
* Constructor/Distructor:
* Created:15_03_2019
*/
_AssetLoader::_AssetLoader()
{
    vertMax.setX(0.0);
    vertMax.setY(0.0);
    vertMax.setZ(0.0);
    vertMin.setX(999.0f);
    vertMin.setY(999.0f);
    vertMin.setZ(999.0f);
    posCounter = 0;
    arrayCounter = 0;

    modelInfo.setIsLoaded(false);
}
_AssetLoader::~_AssetLoader(){}
/*
 *
*/
_AssetLoader::_Model_Info _AssetLoader::getModelInfo()
{
    return this->modelInfo;
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
void _AssetLoader::objLoader(QString pathToFile)
{
    qDebug() << "Loading Model" << pathToFile <<  "vertices.";
    QByteArray qba = _Tools::ReadStringFromQrc(pathToFile).toLocal8Bit();
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
            vertices.push_back(foundf);
            posCounter++;
        }
        temp = "";
        //assigning the max and min values for vertices at the same
        //time that they are beiing assigned.
        if(posCounter >= 3 && (arrayCounter + 2) < vertices.size())
        {
            //minExtent
            posCounter = 0;
            if(vertices[arrayCounter] >= vertMax.x())
                vertMax.setX(vertices[arrayCounter]);
            if(vertices[arrayCounter + 1] >= vertMax.y())
                vertMax.setY(vertices[arrayCounter + 1]);
            if(vertices[arrayCounter + 2] >= vertMax.z())
                vertMax.setZ(vertices[arrayCounter + 2]);
             //maxEntent
            if(vertices[arrayCounter] <= vertMin.x())
                vertMin.setX(vertices[arrayCounter]);
            if(vertices[arrayCounter + 1] <= vertMin.y())
                vertMin.setY(vertices[arrayCounter + 1]);
            if(vertices[arrayCounter + 2] <= vertMin.z())
                vertMin.setZ(vertices[arrayCounter + 2]);
            arrayCounter += 3;
        }
    }

    qDebug() << "Loading Model" << pathToFile <<  "indices.";
    std::string ai = objData.substr(objData.find("f ") + 1);
    std::stringstream ss;
    ss << ai;
    unsigned int foundi;
    while (!ss.eof())
    {
        ss >> temp2;
        if (std::stringstream(temp2) >> foundi)
        {
            indices.push_back(foundi - 1);
        }
        temp2 = "";
    }

    //sets the ModelInfo data at the end of modelfile parse.

    modelInfo.setPath(pathToFile);
    modelInfo.setVertexArray(this->vertices);
    modelInfo.setIndexArray(this->indices);
    modelInfo.setMaxExtents(this->vertMax);
    modelInfo.setMinExtents(this->vertMin);
    modelInfo.setIsLoaded(true);
}

/* //Not in use---future implementation via VAO.
 * Preprocess all models into memory
 * will reduce ovehead on runtime.
 * Created: 31_05_2019
*/
void _AssetLoader::loadAllModelsInfoFromFolder(QString folderName)
{
    foreach(const QString &imageName, QDir(":/"+folderName).entryList())
    {
        modelInfo.setName(imageName);
        modelInfo.setPath(":/"+ folderName +"/" + imageName);
        objLoader(":/"+ folderName +"/" + imageName);
        modelInfo.setVertexArray(this->vertices);
        modelInfo.setIndexArray(this->indices);
//        modelInfoArray.push_back(minfo);
        this->vertices.clear();
        this->indices.clear();
    }
}
/*
• ▌ ▄ ·.       ·▄▄▄▄  ▄▄▄ .▄▄▌  ▪   ▐ ▄ ·▄▄▄           ▄▄ • ▄▄▄ .▄▄▄▄▄   .▄▄ · ▄▄▄ .▄▄▄▄▄
·██ ▐███▪▪     ██▪ ██ ▀▄.▀·██•  ██ •█▌▐█▐▄▄·▪         ▐█ ▀ ▪▀▄.▀·•██     ▐█ ▀. ▀▄.▀·•██
▐█ ▌▐▌▐█· ▄█▀▄ ▐█· ▐█▌▐▀▀▪▄██▪  ▐█·▐█▐▐▌██▪  ▄█▀▄     ▄█ ▀█▄▐▀▀▪▄ ▐█.▪   ▄▀▀▀█▄▐▀▀▪▄ ▐█.▪
██ ██▌▐█▌▐█▌.▐▌██. ██ ▐█▄▄▌▐█▌▐▌▐█▌██▐█▌██▌.▐█▌.▐▌    ▐█▄▪▐█▐█▄▄▌ ▐█▌·   ▐█▄▪▐█▐█▄▄▌ ▐█▌·
▀▀  █▪▀▀▀ ▀█▄▀▪▀▀▀▀▀•  ▀▀▀ .▀▀▀ ▀▀▀▀▀ █▪▀▀▀  ▀█▄▀▪    ·▀▀▀▀  ▀▀▀  ▀▀▀  ▀  ▀▀▀▀  ▀▀▀  ▀▀▀
*/

void _AssetLoader::Model_Info::setName(QString name){
    this->name = name;
}
void _AssetLoader::Model_Info::setPath(QString path){
    this->path = path;
}
void _AssetLoader::Model_Info::setIsLoaded(bool isLoaded){
    this->isLoaded = isLoaded;
}
void _AssetLoader::Model_Info::setVertexArray(std::vector<float> vertexArray){
    this->vertexArray = vertexArray;
}
void _AssetLoader::Model_Info::setIndexArray(std::vector<unsigned int> indexAarray){
    this->indexAarray = indexAarray;
}
void _AssetLoader::Model_Info::setMaxExtents(QVector3D max){
    this->max = max;
}
void _AssetLoader::Model_Info::setMinExtents(QVector3D min){
    this->min = min;
}
QString _AssetLoader::Model_Info::getName() const{
    return this->name;
}
QString _AssetLoader::Model_Info::getPath() const{
    return this->path;
}
std::vector<float> _AssetLoader::Model_Info::getVertices() const{
    return this->vertexArray;
}
std::vector<unsigned int> _AssetLoader::Model_Info::getIndices() const{
    return this->indexAarray;
}
QVector3D _AssetLoader::Model_Info::getMaxExtent() const{
    return this->max;
}
QVector3D _AssetLoader::Model_Info::getMinExtent() const{
    return this->min;
}
QVector3D _AssetLoader::Model_Info::getCentroid() const{
   return this->centroid;
}
bool _AssetLoader::Model_Info::getIsLoaded() const{
    return this->isLoaded;
}
