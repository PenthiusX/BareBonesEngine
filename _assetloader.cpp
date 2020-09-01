/*
 *
 * Author: Aditya
*/



#include "_assetloader.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <sstream>

#include <qdiriterator.h>
#include <QXmlStreamReader>

#include "_tools.h"

//#include <assimp/scene.h>         |assimp does not work , cannot be compiled with Mingw32 , isse is pending in developmennt witht he assimp team|
//#include <assimp/postprocess.h>   | https://github.com/assimp/assimp/issues/3044 |
//#include <assimp/Importer.hpp>

//#include <Qt3DCore/QEntity>
//#include <Qt3DRender>
//#include <QEntityPtr>

#include "_ObjLoader.h"

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

//    const aiScene* scene = importer.ReadFile(":/models/cube.obj",
//            aiProcess_GenSmoothNormals |
//            aiProcess_CalcTangentSpace |
//            aiProcess_Triangulate |
//            aiProcess_JoinIdenticalVertices |
//            aiProcess_SortByPType
//            );

//    if (!scene)
//    {
//        qDebug() << "Error loading file: (assimp:) " << importer.GetErrorString();
//    }
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


glm::vec3 computeNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
    return normalize(glm::cross(b - a, c - a));
}

std::vector<float> _AssetLoader::calculateNormalsOfVertices(std::vector<float>vertices)
{
    std::vector<float> normalVertices;
    //ProceduralNormals-----------
    std::vector <glm::vec3> normalsVec3;
    uint indexCounter = 0;
    bool normalSet = false;
    for(int v = 0 ; v < (int)vertices.size() ; v++)
    {
        indexCounter++;
        if(indexCounter == 9){
            glm::vec3 a = glm::vec3(vertices[v-8],vertices[v-7],vertices[v-6]);
            glm::vec3 b = glm::vec3(vertices[v-5],vertices[v-4],vertices[v-3]);
            glm::vec3 c = glm::vec3(vertices[v-2],vertices[v-1],vertices[v]);

            glm::vec3 n = computeNormal(a,b,c);
            normals.push_back(n.x);
            normals.push_back(n.y);
            normals.push_back(n.z);
            normalsVec3.push_back(n);
            indexCounter = 0;
            normalSet = true;
        }
    }

      //only for compound arrays with nor
     indexCounter = 0; //create array of VertexOffsetnormal array
     uint ncounter = 0;
     for(int v = 0 ; v < (int)vertices.size() ; v++){

         if(indexCounter == 3){
             normalVertices.push_back(normalsVec3[ncounter].x);
             normalVertices.push_back(normalsVec3[ncounter].y);
             normalVertices.push_back(normalsVec3[ncounter].z);
             ncounter++;
             indexCounter = 0;
         }
         normalVertices.push_back(vertices[v]);
          indexCounter++;
     }
     //-----------------------------
     return normalVertices;

//    return normals;
}

/*
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
        if(posCounter >= 3 && (arrayCounter + 2) < (int)vertices.size()){
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
    modelInfo.setVertexArray(calculateNormalsOfVertices(vertices));//setting vertexarry with normal info for this use
//    modelInfo.setNormalArray(calculateNormalsOfVertices(vertices));
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
}

//Loads an Obj format file from the windows file system into memory
void _AssetLoader::extrenalObjLoader(std::string externalFilePath)
{
    objl::Loader Loader;
        // Load .obj File
    bool loadout = Loader.LoadFile(externalFilePath);
    std::vector<VertexInfo> vfa;
    vertMax = glm::vec4(Loader.LoadedVertices[0].Position.X,Loader.LoadedVertices[0].Position.Y,Loader.LoadedVertices[0].Position.Z,0.0);

    if(loadout){
    for(unsigned int i = 0 ; i < Loader.LoadedVertices.size(); i++){
        VertexInfo v;
        v.Position = glm::vec3(Loader.LoadedVertices[i].Position.X,Loader.LoadedVertices[i].Position.Y,Loader.LoadedVertices[i].Position.Z);
        v.Normal = glm::vec3(Loader.LoadedVertices[i].Normal.X,Loader.LoadedVertices[i].Normal.Y,Loader.LoadedVertices[i].Normal.Z);
        v.TexCoords = glm::vec2(Loader.LoadedVertices[i].TextureCoordinate.X,Loader.LoadedVertices[i].TextureCoordinate.Y);
        //Set Max min
        if( Loader.LoadedVertices[i].Position.X > vertMax.x ){vertMax.x = Loader.LoadedVertices[i].Position.X;}
        if( Loader.LoadedVertices[i].Position.Y > vertMax.y ){vertMax.y = Loader.LoadedVertices[i].Position.Y;}
        if( Loader.LoadedVertices[i].Position.Z > vertMax.z ){vertMax.z = Loader.LoadedVertices[i].Position.Z;}

        if( Loader.LoadedVertices[i].Position.X < vertMin.x ){vertMax.x = Loader.LoadedVertices[i].Position.X;}
        if( Loader.LoadedVertices[i].Position.Y < vertMin.y ){vertMax.y = Loader.LoadedVertices[i].Position.Y;}
        if( Loader.LoadedVertices[i].Position.Z < vertMin.z ){vertMax.z = Loader.LoadedVertices[i].Position.Z;}

        modelInfo.setMaxExtents(vertMax);
        modelInfo.setMinExtents(vertMin);
        modelInfo.calcCentroidFromMinMax();
        //
        vfa.push_back(v);
        }
    }
    else{
        qInfo() << externalFilePath.c_str() << "Did not load";
    }

     qInfo() << vertMax.x << vertMax.y << vertMax.z;

     //Needs to set model info min max .
     modelInfo.setVertexInfoArray(vfa);
     modelInfo.setIndexArray(Loader.LoadedIndices);
     modelInfo.setIsLoaded(true);
     modelInfo.setPath(externalFilePath.c_str());

     qInfo() << vertMax.x << vertMax.y << vertMax.z;
}

/* Not in use---
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

_ModelInfo _AssetLoader::generateCubeWithNormals()
{
    std::vector<float> vertices = {
           -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
           -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
           -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

           -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
           -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
           -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

           -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
           -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
           -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
           -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
           -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
           -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

           -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
           -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
           -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

           -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
           -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
           -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
       };

    _ModelInfo m;
//     m.setVertexArray(calculateNormalsOfVertices(vertices));
    m.setVertexArray(vertices);
//     m.setIndexArray(indiceV);
    m.calcMinMaxExtents();
    m.setName("NormalCube");

      return m;
}
