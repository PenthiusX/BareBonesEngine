#include "_assetloader.h"
#include <iostream>
#include <_tools.h>
#include <string>
#include <stdlib.h>
#include <sstream>

#include <qdiriterator.h>
#include <QXmlStreamReader>

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
}
_AssetLoader::~_AssetLoader(){}
/*
 *
*/
_AssetLoader::_Model_Info _AssetLoader::getModelInfo()
{
    return this->minfo;
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
int posCounter = 0;
int arrayCounter = 0;
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
        //assigning the max and min values for vertices
        if(posCounter >= 3 && (arrayCounter + 2) < vertices.size())
        {
            posCounter = 0;
            if(vertices[arrayCounter] >= vertMax.x())
                vertMax.setX(vertices[arrayCounter]);
            if(vertices[arrayCounter + 1] >= vertMax.y())
                vertMax.setY(vertices[arrayCounter + 1]);
            if(vertices[arrayCounter + 2] >= vertMax.z())
                vertMax.setZ(vertices[arrayCounter + 2]);

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

    minfo.path = pathToFile;
    minfo.vertexArray = this->vertices;
    minfo.indexAarray = this->indices;
    minfo.max = this->vertMax;
    minfo.min = this->vertMin;
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
        minfo.name = imageName;
        minfo.path = ":/"+ folderName +"/" + imageName;
        objLoader(":/"+ folderName +"/" + imageName);
        minfo.vertexArray = this->vertices;
        minfo.indexAarray = this->indices;
//        modelInfoArray.push_back(minfo);
        this->vertices.clear();
        this->indices.clear();
    }
}
