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
//    extrenalObjLoader("D:/DiamondPalRepo/DiamondPal/models/cubenormalObj.obj");

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

    if(loadout){
    for(unsigned int i = 0 ; i < Loader.LoadedVertices.size(); i++){
        VertexInfo v;
        v.Position = glm::vec3(Loader.LoadedVertices[i].Position.X,Loader.LoadedVertices[i].Position.Y,Loader.LoadedVertices[i].Position.Z);
        v.Normal = glm::vec3(Loader.LoadedVertices[i].Normal.X,Loader.LoadedVertices[i].Normal.Y,Loader.LoadedVertices[i].Normal.Z);
        v.TexCoords = glm::vec2(Loader.LoadedVertices[i].TextureCoordinate.X,Loader.LoadedVertices[i].TextureCoordinate.Y);
        vfa.push_back(v);
        }

    //Needs to set model info min max .
        modelInfo.setVertexInfoArray(vfa);
        modelInfo.setIndexArray(Loader.LoadedIndices);
        modelInfo.setIsLoaded(true);
    }
    else{
        qInfo() << externalFilePath.c_str() << "Did not load";
    }
/*
    // If so continue
    if (loadout)
    {
        // Create/Open e1Out.txt
        std::ofstream file("e1Out.txt");

        // Go through each loaded mesh and out its contents
        for (int i = 0; i < (int)Loader.LoadedMeshes.size(); i++)
        {
            // Copy one of the loaded meshes to be our current mesh
            objl::Mesh curMesh = Loader.LoadedMeshes[i];

            // Print Mesh Name
            qInfo() << "Mesh " << i << ": " << curMesh.MeshName.c_str() << "\n";

            // Print Vertices
            qInfo() << "Vertices:\n";

            // Go through each vertex and print its number,
            //  position, normal, and texture coordinate
            for (int j = 0; j < (int)curMesh.Vertices.size(); j++)
            {
                qInfo()<<"V" << j << ": " << "P(" << curMesh.Vertices[j].Position.X << ", " << curMesh.Vertices[j].Position.Y << ", " << curMesh.Vertices[j].Position.Z << ") " <<
                                             "N(" << curMesh.Vertices[j].Normal.X << ", " << curMesh.Vertices[j].Normal.Y << ", " << curMesh.Vertices[j].Normal.Z << ") " <<
                                             "TC(" << curMesh.Vertices[j].TextureCoordinate.X << ", " << curMesh.Vertices[j].TextureCoordinate.Y << ")\n";
            }

            // Print Indices
            qInfo() << "Indices:\n";

            // Go through every 3rd index and print the
            //	triangle that these indices represent
            for (int j = 0; j < (int)curMesh.Indices.size(); j += 3)
            {
                qInfo() << "T" << j / 3 << ": " << curMesh.Indices[j] << ", " << curMesh.Indices[j + 1] << ", " << curMesh.Indices[j + 2] << "\n";
            }

            // Print Material
            qInfo() << "Material: " << curMesh.MeshMaterial.name.c_str() << "\n";
            qInfo() << "Ambient Color: " << curMesh.MeshMaterial.Ka.X << ", " << curMesh.MeshMaterial.Ka.Y << ", " << curMesh.MeshMaterial.Ka.Z << "\n";
            qInfo() << "Diffuse Color: " << curMesh.MeshMaterial.Kd.X << ", " << curMesh.MeshMaterial.Kd.Y << ", " << curMesh.MeshMaterial.Kd.Z << "\n";
            qInfo() << "Specular Color: " << curMesh.MeshMaterial.Ks.X << ", " << curMesh.MeshMaterial.Ks.Y << ", " << curMesh.MeshMaterial.Ks.Z << "\n";
            qInfo() << "Specular Exponent: " << curMesh.MeshMaterial.Ns << "\n";
            qInfo() << "Optical Density: " << curMesh.MeshMaterial.Ni << "\n";
            qInfo() << "Dissolve: " << curMesh.MeshMaterial.d << "\n";
            qInfo() << "Illumination: " << curMesh.MeshMaterial.illum << "\n";
            qInfo() << "Ambient Texture Map: " << curMesh.MeshMaterial.map_Ka.c_str() << "\n";
            qInfo() << "Diffuse Texture Map: " << curMesh.MeshMaterial.map_Kd.c_str() << "\n";
            qInfo() << "Specular Texture Map: " << curMesh.MeshMaterial.map_Ks.c_str() << "\n";
            qInfo() << "Alpha Texture Map: " << curMesh.MeshMaterial.map_d.c_str() << "\n";
            qInfo() << "Bump Map: " << curMesh.MeshMaterial.map_bump.c_str() << "\n";

            // Leave a space to separate from the next mesh
            file << "\n";
        }

        // Close File
        file.close();
    }
    // If not output an error
    else
    {
        // Create/Open e1Out.txt
        std::ofstream file("e1Out.txt");

        // Output Error
        file << "Failed to Load File. May have failed to find it or it was not an .obj file.\n";

        // Close File
        file.close();
    }
   */
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
//    std::vector<float> vertices = {
//        1.000000, 1.000000, 1.000000, 0.000000, -0.00000,1.000000,
//        -1.00000, 1.00000,  1.00000,  0.00000, -0.0000, 1.00000,
//        -1.00000,-1.0000,   1.0000,   0.0000, -0.000,1.0000,
//        1.000000,-1.00000,  1.00000,  0.00000, -0.0000, 1.00000,
//        1.000000,-1.00000, -1.0000,   0.0000, -1.000,00.0000,
//        1.000000,-1.00000,  1.00000,  0.00000, -1.0000, 0.00000,
//        -1.00000,-1.0000,   1.0000,   0.0000, -1.000,00.0000,
//        -1.00000,-1.0000,  -1.000,    0.000, -1.00, 0.000,
//        -1.00000,-1.0000,  -1.000,   -1.00, -0.0, 0.00,
//        -1.00000,-1.0000,   1.0000,  -1.000, -0.00, 0.000,
//        -1.00000, 1.00000,  1.00000, -1.0000,-0.000, 0.0000,
//        -1.00000, 1.00000, -1.0000,  -1.000, -0.00, 0.000,
//        -1.00000, 1.00000, -1.0000,   0.0000, 0.0000, -1.0000,
//        1.000000, 1.000000,-1.00000,  0.00000, 0.00000,-1.00000,
//        1.000000,-1.00000, -1.0000,   0.0000, 0.0000, -1.0000,
//        -1.00000,-1.0000,  -1.000,    0.000, 0.000,0 -1.000,
//        1.000000, 1.000000,-1.00000,  1.00000, -0.0000, 0.00000,
//        1.000000, 1.000000, 1.000000, 1.000000, -0.00000,0.000000,
//        1.000000,-1.00000,  1.00000,  1.00000, -0.0000, 0.00000,
//        1.000000,-1.00000, -1.0000,   1.0000, -0.000, 0.0000,
//        -1.00000, 1.00000, -1.0000,   0.0000, 1.0000, 0.00000,
//        -1.00000, 1.00000,  1.00000,  0.00000, 1.00000,0.000000,
//        1.000000, 1.000000, 1.000000, 0.000000, 1.000000,.000000 ,
//        1.000000, 1.000000,-1.00000,  0.00000, 1.00000,0.000000
//        };

//    std::vector<uint> indiceV = {
//        4,0, 1, 2, 3,
//        4,4, 5, 6, 7,
//        4,8, 9, 10, 11,
//        4,12, 13, 14, 15,
//        4,16, 17, 18, 19,
//        4,20, 21, 22, 23
//    };

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
