#ifndef _ASSETLOADER_H
#define _ASSETLOADER_H
#include <vector>
#include <qstring.h>
#include <qvector3d.h>
//#include<glm/glm.hpp>
//#include<glm/gtc/quaternion.hpp>

//#include <assimp/Importer.hpp>      // C++ importer interface
//#include <assimp/scene.h>           // Output data structure
//#include <assimp/postprocess.h>     // Post processing flags
//#include <assimp/Logger.hpp>
//#include <assimp/DefaultLogger.hpp>
//#include<assimp/quaternion.h>
//#include<assimp/vector3.h>
//#include<assimp/matrix4x4.h>

#include "_modelinfo.h"

/*
 * Class: _AssetLoader
 * class sets values needed by the viewMatrix
 * for setting of values on Init or realtime
 * Author: Aditya
*/

class _AssetLoader
{
    public:
        _AssetLoader();
        ~_AssetLoader();
        void setModelInfo(_ModelInfo minfo);
        _ModelInfo getModelInfo();
        void objLoader(QString pathToFile);
        void extrenalObjLoader(std::string externalFilePath);//an unideal external file loader , not inteded for embeded executables.
        void assimpLoader(std::string externalFilePath);

        void loadAllModelsInfoFromFolder(QString folderName);//not in use
        _ModelInfo generateQuad();
        _ModelInfo generateTri();
        _ModelInfo generateTri(glm::vec3 p1,glm::vec3 p2,glm::vec3 p3);
        _ModelInfo generateCubeWithNormals();
        std::vector<float> calculateNormalsOfVertices(std::vector<float>vertices);


        //Skeletal Anim
        //Subject to Cleanup
        int m_BoneCounter = 0;
        auto& GetBoneInfoMap() {return m_BoneInfoMap;}
        int& GetBoneCount() {return m_BoneCounter;}

    private:
        _ModelInfo modelInfo;
        //
        std::vector<float> vertices;
        std::vector<float>normalVertices;//replaces the vertices array
        std::vector<uint> indices;
        std::vector<float> normals;
        std::vector<float> uvs;
        //
        glm::vec4 vertMin;
        glm::vec4 vertMax;

        int posCounter;
        int arrayCounter;

        //Assimp
        void loadAssimpScene(const aiScene* pScene);

        //Skeletal Anim
        //Subject to Cleanup
        std::map<std::string, BoneInfo> m_BoneInfoMap; //
};

#endif // _ASSETLOADER_H
