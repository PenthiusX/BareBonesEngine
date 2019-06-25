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
    //    modelInfo.calcCentroidFromMinMax();
    calcMinMaxExtents();
}
_ModelInfo _AssetLoader::getModelInfo(){
    return modelInfo;
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

    qDebug() << "Loading Model" << pathToFile <<  "indices.";
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
    modelInfo.setCentroid(calcCentroidFromMinMax());
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
          * Created: 31_05_2019
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
        //      modelInfoArray.push_back(minfo);
        vertices.clear();
        indices.clear();
    }
}

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

/*
          * Created: 12_06_2019
         */
void _AssetLoader::calcMinMaxExtents()
{
    std::vector<float> v = modelInfo.getVertexArray();
    for(uint i = 0 ; i < v.size() ; i += 3)
    {
        if(v[i] >= vertMax.x)
            vertMax.x=(v[i]);
        if(v[i + 1] >= vertMax.y)
            vertMax.y=(v[i + 1]);
        if(v[i + 2] >= vertMax.z)
            vertMax.z=(v[i + 2]);
        vertMax.w = 0.0f;
        //maxEntent
        if(v[i] <= vertMin.x)
            vertMin.x=(v[i]);
        if(v[i + 1] <= vertMin.y)
            vertMin.y=(v[i + 1]);
        if(v[i + 2] <= vertMin.z)
            vertMin.z=(v[i + 2]);
        vertMin.w = 0.0f;
    }
    modelInfo.setMinExtents(vertMin);
    modelInfo.setMaxExtents(vertMax);
    modelInfo.setCentroid(calcCentroidFromMinMax());
}
/*
 * Created: 12_06_2019
 */
glm::vec4 _AssetLoader::calcCentroidFromMinMax()
{
    glm::vec4 centroid;
    centroid.x = (vertMax.x + vertMin.x)*0.5f;
    centroid.y = (vertMax.y + vertMin.y)*0.5f;
    centroid.z = (vertMax.z + vertMin.z)*0.5f;
    centroid.w = 0.0f;
    return centroid;
}


