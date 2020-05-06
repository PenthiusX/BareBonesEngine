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

void _AssetLoader::setModelInfo(_ModelInfo minfo){
    modelInfo = minfo;
    if(modelInfo.getVertexArray().size() > 0 && modelInfo.getIndexArray().size() > 0)
        modelInfo.setIsLoaded(true);
}
_ModelInfo _AssetLoader::getModelInfo(){
    return modelInfo;
}
/*
* Function:objLoader(QString pathToFile)
* takes a Qstring path to qrc file and then parses through 
* a string array to get the approptie onfo to render the model
*/
void _AssetLoader::objLoader(QString pathToFile)
{
    qInfo() << "Loading Model" << pathToFile <<  "vertices.";
    QByteArray qba = _Tools::ReadStringFromQrc(pathToFile).toLocal8Bit();
    const char* p = qba;

    std::string objData = p;
    std::string temp = "0000000";
    std::string temp2 = "0000000";

    uint start = objData.find("v ");
    uint finish = objData.find("s ");

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
        //time that they are beiing assigned. reduces time complexity
        if(posCounter >= 3 && (arrayCounter + 2) < vertices.size()){
            //minExtent
            posCounter = 0;
            if(vertices[arrayCounter] >= vertMax.x)
                vertMax.x = (vertices[arrayCounter]);
            if(vertices[arrayCounter + 1] >= vertMax.y)
                vertMax.y = (vertices[arrayCounter + 1]);
            if(vertices[arrayCounter + 2] >= vertMax.z)
                vertMax.z = (vertices[arrayCounter + 2]);
            //maxEntent
            if(vertices[arrayCounter] <= vertMin.x)
                vertMin.x = (vertices[arrayCounter]);
            if(vertices[arrayCounter + 1] <= vertMin.y)
                vertMin.y = (vertices[arrayCounter + 1]);
            if(vertices[arrayCounter + 2] <= vertMin.z)
                vertMin.z = (vertices[arrayCounter + 2]);
            arrayCounter += 3;
        }
    }

    qInfo() << "Loading Model" << pathToFile <<  "indices.";
    std::string ai = objData.substr(objData.find("f ") + 1);
    std::stringstream ss;
    ss << ai;
    uint foundi;
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
    modelInfo.setVertexArray(vertices);
    modelInfo.setIndexArray(indices);
    modelInfo.setMaxExtents(vertMax);
    modelInfo.setMinExtents(vertMin);
    modelInfo.calcCentroidFromMinMax();
    modelInfo.setIsLoaded(true);
    qInfo()<<"--------------MODEL INFO-------------------------------";
    qInfo()<< "Path" << modelInfo.getPath();
    qInfo()<< "VertexArray" << modelInfo.getVertexArray().size();
    qInfo()<< "IndexArray" << modelInfo.getIndexArray().size();
    qInfo()<< "MaxExtents" << modelInfo.getMaxExtent().x << modelInfo.getMaxExtent().y << modelInfo.getMaxExtent().z;
    qInfo()<< "MinExtents" << modelInfo.getMinExtent().x << modelInfo.getMinExtent().y << modelInfo.getMinExtent().z;
    qInfo()<< "Centroid" << modelInfo.getCentroid().x << modelInfo.getCentroid().y << modelInfo.getCentroid().z;
    qInfo()<< "IsLoaded" << modelInfo.getIsLoaded();
    qInfo()<<"--------------------------------------------------------";
    qInfo()<<"--------------------------------------------------------";
}

/* Not in use---future implementation for VAO based optimisation.
* Preprocess all models into memory
* will reduce ovehead on runtime.
*/
void _AssetLoader::loadAllModelsInfoFromFolder(QString folderName)
{
    foreach(const QString &imageName, QDir(":/"+folderName).entryList())
    {
        modelInfo.setName(imageName);
        modelInfo.setPath(":/"+ folderName +"/" + imageName);
        objLoader(":/"+ folderName +"/" + imageName);
        modelInfo.setVertexArray(vertices);
        modelInfo.setIndexArray(indices);
        //modelInfoArray.push_back(minfo);
        vertices.clear();
        indices.clear();
    }
}
/*
*/
_ModelInfo _AssetLoader::generateQuad()
{
    //Code to be excluded only for Test purposes
    //Hard coded vertices and indices
    std::vector<float> vertsV = {
        1.0,   1.0,  0.0f,	// top right
        1.0f, -1.0f, 0.0f,  // bottom right
        -1.0f, -1.0f, 0.0f, // bottom left
        -1.0f,  1.0f, 0.0f  // top left
    };
    std::vector<uint> indiceV = {0, 1, 3,
                                 1, 2, 3 };

    _ModelInfo m;
    m.setVertexArray(vertsV);
    m.setIndexArray(indiceV);
    m.calcMinMaxExtents();
    m.setName("Quad");

    return m;
}

_ModelInfo _AssetLoader::generateTri()
{
    //Code to be excluded only for Test purposes
    //Hard coded vertices and indices
    std::vector<float> vertsV = {
        1.0,   1.0,  0.0f,	// top right
        1.0f, -1.0f, 0.0f,  // bottom right
        -1.0f, -1.0f, 0.0f, // bottom left
    };
    std::vector<uint> indiceV = {0, 1, 2};

    _ModelInfo m;
    m.setVertexArray(vertsV);
    m.setIndexArray(indiceV);
    m.calcMinMaxExtents();
    m.setName("Tri");

    return m;
}

_ModelInfo _AssetLoader::generateTri(glm::vec3 p1,glm::vec3 p2,glm::vec3 p3)
{
    //Code to be excluded only for Test purposes
    //Hard coded vertices and indices
    std::vector<float> vertsV = {
        p1.x,   p1.y,  p1.z,
        p2.x,   p2.y,  p2.z,
        p3.x,   p3.y,  p3.z,
    };
    std::vector<uint> indiceV = {0, 1, 2};

    _ModelInfo m;
    m.setVertexArray(vertsV);
    m.setIndexArray(indiceV);
    m.calcMinMaxExtents();
    m.setName("Tri");

    return m;
}
