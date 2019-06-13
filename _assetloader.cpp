#include "_assetloader.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <sstream>

#include <qdiriterator.h>
#include <QXmlStreamReader>

#include "_tools.h"

/*
• ▌ ▄ ·.       ·▄▄▄▄  ▄▄▄ .▄▄▌  ▪   ▐ ▄ ·▄▄▄          ·▄▄▄▄• ▄▌ ▐ ▄  ▄▄· ▄▄▄▄▄▪         ▐ ▄ .▄▄ ·
·██ ▐███▪▪     ██▪ ██ ▀▄.▀·██•  ██ •█▌▐█▐▄▄·▪         ▐▄▄·█▪██▌•█▌▐█▐█ ▌▪•██  ██ ▪     •█▌▐█▐█ ▀.
▐█ ▌▐▌▐█· ▄█▀▄ ▐█· ▐█▌▐▀▀▪▄██▪  ▐█·▐█▐▐▌██▪  ▄█▀▄     ██▪ █▌▐█▌▐█▐▐▌██ ▄▄ ▐█.▪▐█· ▄█▀▄ ▐█▐▐▌▄▀▀▀█▄
██ ██▌▐█▌▐█▌.▐▌██. ██ ▐█▄▄▌▐█▌▐▌▐█▌██▐█▌██▌.▐█▌.▐▌    ██▌.▐█▄█▌██▐█▌▐███▌ ▐█▌·▐█▌▐█▌.▐▌██▐█▌▐█▄▪▐█
▀▀  █▪▀▀▀ ▀█▄▀▪▀▀▀▀▀•  ▀▀▀ .▀▀▀ ▀▀▀▀▀ █▪▀▀▀  ▀█▄▀▪    ▀▀▀  ▀▀▀ ▀▀ █▪·▀▀▀  ▀▀▀ ▀▀▀ ▀█▄▀▪▀▀ █▪ ▀▀▀▀
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
void _AssetLoader::Model_Info::setCentroid(glm::vec4 cent){
    this->centroid = cent;
}
void _AssetLoader::Model_Info::setMaxExtents(glm::vec4 max){
    this->max = max;
}
void _AssetLoader::Model_Info::setMinExtents(glm::vec4 min){
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
glm::vec4 _AssetLoader::Model_Info::getMaxExtent() const{
    return this->max;
}
glm::vec4 _AssetLoader::Model_Info::getMinExtent() const{
    return this->min;
}
glm::vec4 _AssetLoader::Model_Info::getCentroid() const{
    return this->centroid;
}
bool _AssetLoader::Model_Info::getIsLoaded() const {
    return this->isLoaded;
}

/*
 ▄▄▄· .▄▄ · .▄▄ · ▄▄▄ .▄▄▄▄▄▄▄▌         ▄▄▄· ·▄▄▄▄  ▄▄▄ .▄▄▄      ·▄▄▄▄• ▄▌ ▐ ▄  ▄▄· ▄▄▄▄▄▪         ▐ ▄ .▄▄ ·
▐█ ▀█ ▐█ ▀. ▐█ ▀. ▀▄.▀·•██  ██•  ▪     ▐█ ▀█ ██▪ ██ ▀▄.▀·▀▄ █·    ▐▄▄·█▪██▌•█▌▐█▐█ ▌▪•██  ██ ▪     •█▌▐█▐█ ▀.
▄█▀▀█ ▄▀▀▀█▄▄▀▀▀█▄▐▀▀▪▄ ▐█.▪██▪   ▄█▀▄ ▄█▀▀█ ▐█· ▐█▌▐▀▀▪▄▐▀▀▄     ██▪ █▌▐█▌▐█▐▐▌██ ▄▄ ▐█.▪▐█· ▄█▀▄ ▐█▐▐▌▄▀▀▀█▄
▐█ ▪▐▌▐█▄▪▐█▐█▄▪▐█▐█▄▄▌ ▐█▌·▐█▌▐▌▐█▌.▐▌▐█ ▪▐▌██. ██ ▐█▄▄▌▐█•█▌    ██▌.▐█▄█▌██▐█▌▐███▌ ▐█▌·▐█▌▐█▌.▐▌██▐█▌▐█▄▪▐█
 ▀  ▀  ▀▀▀▀  ▀▀▀▀  ▀▀▀  ▀▀▀ .▀▀▀  ▀█▄▀▪ ▀  ▀ ▀▀▀▀▀•  ▀▀▀ .▀  ▀    ▀▀▀  ▀▀▀ ▀▀ █▪·▀▀▀  ▀▀▀ ▀▀▀ ▀█▄▀▪▀▀ █▪ ▀▀▀▀
*/
/*
* Constructor/Distructor:
* Created:15_03_2019
*/
_AssetLoader::_AssetLoader()
{
    vertMax.x = (0.0);
    vertMax.y = (0.0);
    vertMax.z = (0.0);
    vertMin.x = (999.0f);
    vertMin.y = (999.0f);
    vertMin.z = (999.0f);
    posCounter = 0;
    arrayCounter = 0;
    modelInfo.setIsLoaded(false);
}
_AssetLoader::~_AssetLoader(){}

void _AssetLoader::setModelInfo(_AssetLoader::Model_Info minfo){
    this->modelInfo = minfo;
}
_AssetLoader::Model_Info _AssetLoader::getModelInfo(){
    return this->modelInfo;
}
/*
* Function:getAssetVertices()
* returns the vertices array for the model obeject
* form the model object loader
* Created:15_03_2019
*/
std::vector<float> _AssetLoader::getAssetVertices(){
    return this->vertices;
}
/*
* Function:getAssetIndices()
* returns the indices array which gets 
* filled in the objloader.
* Created:15_03_2019
*/
std::vector<unsigned int> _AssetLoader::getAssetIndices(){
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

    unsigned int start = objData.find("v ");
    unsigned int finish = objData.find("s ");

    std::string av = objData.substr(start ,finish - start);
    std::stringstream ssv;
    ssv << av;
    float foundf;
    while (!ssv.eof()){
        ssv >> temp;
        if (std::stringstream(temp) >> foundf){
            vertices.push_back(foundf);
            posCounter++;
        }
        temp = "";
        //assigning the max and min values for vertices at the same
        //time that they are beiing assigned.
        if(posCounter >= 3 && (arrayCounter + 2) < vertices.size()){
            //minExtent
            this->posCounter = 0;
            if(vertices[arrayCounter] >= this->vertMax.x)
                this->vertMax.x = (this->vertices[arrayCounter]);
            if(vertices[arrayCounter + 1] >= this->vertMax.y)
                this->vertMax.y = (this->vertices[arrayCounter + 1]);
            if(vertices[arrayCounter + 2] >= this->vertMax.z)
                this->vertMax.z = (this->vertices[arrayCounter + 2]);
            //maxEntent
            if(vertices[arrayCounter] <= this->vertMin.x)
                this->vertMin.x = (vertices[arrayCounter]);
            if(vertices[arrayCounter + 1] <= this->vertMin.y)
                this->vertMin.y = (vertices[arrayCounter + 1]);
            if(vertices[arrayCounter + 2] <= this->vertMin.z)
                this->vertMin.z = (vertices[arrayCounter + 2]);
            this->arrayCounter += 3;
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
            this->indices.push_back(foundi - 1);
        }
        temp2 = "";
    }
    //sets the ModelInfo data at the end of modelfile parse.
    this->modelInfo.setPath(pathToFile);
    this->modelInfo.setVertexArray(this->vertices);
    this->modelInfo.setIndexArray(this->indices);
    this->modelInfo.setMaxExtents(this->vertMax);
    this->modelInfo.setMinExtents(this->vertMin);
    this->modelInfo.setCentroid(calcCentroidFromMinMax());
    this->modelInfo.setIsLoaded(true);
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
        this->modelInfo.setName(imageName);
        this->modelInfo.setPath(":/"+ folderName +"/" + imageName);
        objLoader(":/"+ folderName +"/" + imageName);
        this->modelInfo.setVertexArray(this->vertices);
        this->modelInfo.setIndexArray(this->indices);
        //      modelInfoArray.push_back(minfo);
        this->vertices.clear();
        this->indices.clear();
    }
}

/*
 * Created: 12_06_2019
*/
void _AssetLoader::calcMinMaxExtents()
{
    std::vector<float> v = this->modelInfo.getVertices();
    for(unsigned int i = 0 ; i < v.size() ; i += 3)
    {
        if(v[i] >= this->vertMax.x)
            this->vertMax.x=(v[i]);
        if(v[i + 1] >= this->vertMax.y)
            this->vertMax.y=(v[i + 1]);
        if(v[i + 2] >= this->vertMax.z)
            this->vertMax.z=(v[i + 2]);
        //maxEntent
        if(v[i] <= this->vertMin.x)
            this->vertMin.x=(v[i]);
        if(v[i + 1] <= this->vertMin.y)
            this->vertMin.y=(v[i + 1]);
        if(v[i + 2] <= this->vertMin.z)
            this->vertMin.z=(v[i + 2]);
    }

    this->modelInfo.setMinExtents(this->vertMin);
    this->modelInfo.setMaxExtents(this->vertMax);
    this->modelInfo.setCentroid(calcCentroidFromMinMax());
}

glm::vec4 _AssetLoader::calcCentroidFromMinMax()
{
    glm::vec4 centroid;
    centroid.x = (this->vertMax.x + this->vertMin.x)*0.5f;
    centroid.y = (this->vertMax.y + this->vertMin.y)*0.5f;
    centroid.z = (this->vertMax.z + this->vertMin.z)*0.5f;
    return centroid;
}


